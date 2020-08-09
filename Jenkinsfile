def get_sonarqube_unresolved_issues(report_task_file){
    script{
        if (! fileExists(report_task_file)){
            error "File not found ${report_task_file}"
        }
        def props = readProperties  file: report_task_file
        def response = httpRequest url : props['serverUrl'] + "/api/issues/search?componentKeys=" + props['projectKey'] + "&resolved=no"
        def outstandingIssues = readJSON text: response.content
        return outstandingIssues
    }
}

pipeline {
    agent none
    parameters{
        booleanParam(name: "RUN_CHECKS", defaultValue: true, description: "Run checks on code")
        booleanParam(name: "USE_SONARQUBE", defaultValue: true, description: "Send data checks data to SonarQube")
        booleanParam(name: "BUILD_DOCUMENTATION", defaultValue: false, description: "Build documentation")
        booleanParam(name: "PACKAGE", defaultValue: false, description: "Create distribution packages")
    }
    stages {
        stage("Checks"){
            when{
                equals expected: true, actual: params.RUN_CHECKS
            }
            stages{
                stage("Static Analysis for C Code"){
                    parallel{
                        stage("Clang Tidy"){
                            agent{
                                dockerfile {
                                    filename 'ci/dockerfiles/static_analysis/clang_tidy/Dockerfile'
                                    additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                                    label "linux"
                                }

                            }
                            steps{
                                tee('logs/clang-tidy_debug.log') {
                                    catchError(buildResult: 'SUCCESS', message: 'Clang Tidy found issues', stageResult: 'UNSTABLE') {
                                        sh  '''cmake -B ./build/debug/ -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON
                                               run-clang-tidy -clang-tidy-binary clang-tidy -p ./build/debug/
                                               '''
                                    }
                                }
                            }
                            post{
                                always {
                                    recordIssues(tools: [clangTidy(pattern: 'logs/clang-tidy_debug.log')])
                                }
                                cleanup{
                                    cleanWs(
                                        deleteDirs: true,
                                        patterns: [
                                            [pattern: 'build/', type: 'INCLUDE'],
                                            [pattern: 'logs/', type: 'INCLUDE'],
                                        ]
                                    )
                                }
                            }
                        }
                        stage("cppcheck"){
                            agent{
                              dockerfile {
                                filename 'ci/dockerfiles/static_analysis/cppcheck/Dockerfile'
                                additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                                label "linux"
                              }
                            }
                            steps{
                                catchError(buildResult: 'SUCCESS', message: 'cppcheck found issues', stageResult: 'UNSTABLE') {
                                    sh(
                                        label: "Running cppcheck",
                                        script: '''cmake -B ./build/debug/ -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON
                                                   mkdir -p logs
                                                   cppcheck --error-exitcode=1 --project=build/debug/compile_commands.json --enable=all  -ibuild/debug/_deps --xml --output-file=logs/cppcheck_debug.xml
                                                   '''
                                    )
                                }
                            }
                            post{
                                always {
                                    recordIssues(tools: [cppCheck(pattern: 'logs/cppcheck_debug.xml')])
                                }
                                cleanup{
                                    cleanWs(
                                        deleteDirs: true,
                                        patterns: [
                                            [pattern: 'build/', type: 'INCLUDE'],
                                            [pattern: 'logs/', type: 'INCLUDE'],
                                        ]
                                    )
                                }
                            }
                        }
                    }
                }
                stage("Run Tests on C code"){
                    agent{
                        dockerfile {
                            filename 'ci/dockerfiles/conan/dockerfile'
                            additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                            label "linux"
                        }
                    }
                    stages{
                        stage("Build Debug Version for Testing"){
                            steps{
                                tee("logs/cmakebuild.log"){
                                    cmakeBuild(
                                        buildDir: 'build/debug',
                                        buildType: 'Debug',
                                        cleanBuild: true,
                                        installation: 'InSearchPath',
                                        cmakeArgs: '\
                                                -DCMAKE_C_FLAGS_DEBUG="-fprofile-arcs -ftest-coverage" \
                                                -DCMAKE_EXE_LINKER_FLAGS="-fprofile-arcs -ftest-coverage" \
                                                -DCMAKE_C_FLAGS="-Wall -Wextra" \
                                                -DVALGRIND_COMMAND_OPTIONS="--xml=yes --xml-file=mem-%p.memcheck" \
                                                -Dlibvisvid_TESTS:BOOL=ON \
                                                -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON',
                                        steps: [
                                            [args: '--target test-visvid', withCmake: true],
                                            [args: '--target test-visvid-internal', withCmake: true],
                                        ]
                                    )
                                }
                            }
                            post{
                                always{
                                    recordIssues(tools: [gcc(pattern: 'logs/cmakebuild.log')])
                                }
                            }
                        }
                        stage("Run ctests"){
                            parallel{
                                stage("Run CTest"){
                                    steps{
                                        ctest(
                                            arguments: "--output-on-failure --no-compress-output -T Test",
                                            installation: 'InSearchPath',
                                            workingDir: "build/debug"
                                        )
                                    }
                                    post{
                                        always{
                                            xunit(
                                                testTimeMargin: '3000',
                                                thresholdMode: 1,
                                                thresholds: [
                                                    failed(),
                                                    skipped()
                                                ],
                                                tools: [
                                                    CTest(
                                                        deleteOutputFiles: true,
                                                        failIfNotNew: true,
                                                        pattern: "build/debug/Testing/**/*.xml",
                                                        skipNoTestFiles: true,
                                                        stopProcessingIfError: true
                                                    )
                                                ]
                                            )
                                        }
                                    }
                                }
                                stage("CTest: Coverage"){
                                    steps{
                                        ctest(
                                            arguments: "-T coverage",
                                            installation: 'InSearchPath',
                                            workingDir: 'build/debug'
                                        )
                                    }
                                    post{
                                        always{
                                            sh(label: "Generating coverage report in Coberatura xml file format",
                                               script: """mkdir -p reports/coverage
                                                          gcovr -r ./ --xml -o reports/coverage/coverage.xml build/debug
                                                          """

                                            )
                                            publishCoverage(
                                                adapters: [coberturaAdapter('reports/coverage/coverage.xml')],
                                                sourceFileResolver: sourceFiles('STORE_LAST_BUILD'),
                                                tag: "AllCoverage"
                                            )
                                        }
                                    }
                                }
                                stage("CTest: MemCheck"){
                                    steps{
                                        script{
                                            def cores = sh(
                                                label: 'looking up number of cores',
                                                returnStdout: true,
                                                script: 'grep -c ^processor /proc/cpuinfo'
                                                ).trim()
                                            ctest(
                                                arguments: "-T memcheck -j${cores}",
                                                installation: 'InSearchPath',
                                                workingDir: 'build/debug'
                                            )
                                        }
                                    }
                                    post{
                                        always{
                                            publishValgrind(
                                                failBuildOnInvalidReports: false,
                                                failBuildOnMissingReports: false,
                                                failThresholdDefinitelyLost: '',
                                                failThresholdInvalidReadWrite: '',
                                                failThresholdTotal: '',
                                                pattern: 'build/debug/tests/**/*.memcheck',
                                                publishResultsForAbortedBuilds: false,
                                                publishResultsForFailedBuilds: false,
                                                sourceSubstitutionPaths: '',
                                                unstableThresholdDefinitelyLost: '',
                                                unstableThresholdInvalidReadWrite: '',
                                                unstableThresholdTotal: ''
                                            )
                                            archiveArtifacts "build/debug/Testing/**/DynamicAnalysis.xml"
                                        }
                                    }
                                }
                            }
                        }
                    }
                    post{
                        cleanup{
                            cleanWs(
                                deleteDirs: true,
                                patterns: [
                                    [pattern: 'generatedJUnitFiles/', type: 'INCLUDE'],
                                    [pattern: 'build/', type: 'INCLUDE'],
                                    [pattern: 'reports/', type: 'INCLUDE'],
                                    [pattern: 'logs/', type: 'INCLUDE']
                                ]
                            )
                        }
                    }
                }
                stage("Python Analysis"){
                    agent{
                        dockerfile {
                            filename 'ci/dockerfiles/python/linux/Dockerfile'
                            additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                            label "linux"
                        }
                    }
                    stages{
                        stage("Build Python Extension for Testing"){
                            steps{
                                sh(
                                    label: "Running Python setup script to build wheel and sdist",
                                    script: "python setup.py build build_ext --inplace"
                                    )
                            }
                        }
                        stage("Running Checks on Python Code"){
                            parallel{
                                stage("mypy"){
                                    steps{
                                        catchError(buildResult: 'SUCCESS', message: 'MyPy found issues', stageResult: 'UNSTABLE') {
                                            tee("logs/mypy.log"){
                                                sh(
                                                    label: "Running MyPy",
                                                    script: "cd src/applications/pyvisvid && mypy -p pyvisvid"
                                                )
                                            }
                                        }
                                    }
                                    post{
                                        always {
                                            recordIssues(tools: [myPy(pattern: "logs/mypy.log")])
                                        }
                                    }
                                }
                                stage("Flake8") {
                                    steps{
                                        catchError(buildResult: "SUCCESS", message: 'Flake8 found issues', stageResult: "UNSTABLE") {
                                            sh script: '''mkdir -p logs
                                                          flake8 src/applications/pyvisvid --tee --output-file=logs/flake8.log
                                                          '''
                                        }
                                    }
                                    post {
                                        always {
                                              stash includes: 'logs/flake8.log', name: "FLAKE8_REPORT"
                                              recordIssues(tools: [flake8(pattern: 'logs/flake8.log')])
                                        }
                                    }
                                }
                                stage("Run PyTest Unit Tests"){
                                    steps{
                                        catchError(buildResult: "UNSTABLE", message: 'Did not pass all pytest tests', stageResult: "UNSTABLE") {
                                            sh(
                                                script: '''mkdir -p logs
                                                           mkdir -p reports/tests/pytest
                                                           (cd src/applications/pyvisvid && coverage run  --source=../../../src/applications/pyvisvid -m pytest ../../../tests/pyvisvid/ -p no:cacheprovider --junitxml=../../../reports/pytest-junit.xml)
                                                           (cd src/applications/pyvisvid && coverage xml -o ../../../reports/coverage-reports/pythoncoverage-pytest.xml )
                                                           '''
                                            )
                                        }
                                    }
                                    post {
                                        always {
                                            junit "reports/pytest-junit.xml"
                                        }
                                    }
                                }
                                stage("Pylint") {
                                    steps{
                                        tee("reports/pylint.txt"){
                                            catchError(buildResult: 'SUCCESS', message: 'Pylint found issues', stageResult: 'UNSTABLE') {
                                                sh(
                                                    script: '''mkdir -p reports
                                                               (cd src/applications/pyvisvid && pylint pyvisvid  -r n --msg-template="{path}:{line}: [{msg_id}({symbol}), {obj}] {msg}")
                                                               ''',
                                                    label: "Running pylint"
                                                )
                                            }
                                        }
                                        tee("reports/pylint_issues.txt"){
                                             sh(
                                                label: "Running pylint for sonarqube",
                                                script: '''(cd src/applications/pyvisvid && pylint pyvisvid  -r n --msg-template="{path}:{module}:{line}: [{msg_id}({symbol}), {obj}] {msg}")''',
                                                returnStatus: true
                                             )
                                        }
                                    }
                                    post{
                                        always{
                                            stash includes: "reports/pylint_issues.txt,reports/pylint.txt", name: 'PYLINT_REPORT'
                                            recordIssues(tools: [pyLint(pattern: 'reports/pylint.txt')])
                                        }
                                    }
                                }
                            }
                        }
                    }
                    post{
                        cleanup{
                            cleanWs(
                                deleteDirs: true,
                                patterns: [
                                    [pattern: 'build/', type: 'INCLUDE'],
                                    [pattern: 'reports/', type: 'INCLUDE'],
                                    [pattern: 'logs/', type: 'INCLUDE']
                                ]
                            )
                        }
                    }
                }
                stage("Submit results to SonarCloud"){
                    agent{
                        dockerfile {
                            filename 'ci/dockerfiles/sonar-scanner/Dockerfile'
                            label "linux"
                        }
                    }
                    when{
                        equals expected: true, actual: params.USE_SONARQUBE
                        beforeAgent true
                        beforeOptions true
                    }
                    steps{
                        unstash "PYLINT_REPORT"
                        sh (script: '''cmake -B ./build -S ./ -D CMAKE_C_FLAGS="-Wall -Wextra -fprofile-arcs -ftest-coverage" -D CMAKE_CXX_FLAGS="-Wall -Wextra -fprofile-arcs -ftest-coverage" -D libvisvid_TESTS:BOOL=ON -D CMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_OUTPUT_EXTENSION_REPLACE:BOOL=ON
                                       (cd build && build-wrapper-linux-x86-64 --out-dir build_wrapper_output_directory make clean all)
                                       '''
                           )
                        script{
                            withSonarQubeEnv(installationName:"sonarcloud", credentialsId: 'sonarcloud-visvid') {
                                if (env.CHANGE_ID){
                                    sh(
                                        label: "Running Sonar Scanner",
                                        script:"sonar-scanner  -Dsonar.buildString=\"${env.BUILD_TAG}\" -Dsonar.pullrequest.key=${env.CHANGE_ID} -Dsonar.pullrequest.base=${env.CHANGE_TARGET} -Dsonar.cfamily.cache.enabled=false -Dsonar.cfamily.threads=1"
                                        )
                                } else {
                                    sh(
                                        label: "Running Sonar Scanner",
                                        script: "sonar-scanner -Dsonar.buildString=\"${env.BUILD_TAG}\" -Dsonar.branch.name=${env.BRANCH_NAME} -Dsonar.cfamily.cache.enabled=false -Dsonar.cfamily.threads=1"
                                        )
                                }
                            }
                            timeout(time: 1, unit: 'HOURS') {
                                def sonarqube_result = waitForQualityGate(abortPipeline: false)
                                if (sonarqube_result.status != 'OK') {
                                    unstable "SonarQube quality gate: ${sonarqube_result.status}"
                                }
                                def outstandingIssues = get_sonarqube_unresolved_issues(".scannerwork/report-task.txt")
                                writeJSON file: 'reports/sonar-report.json', json: outstandingIssues
                            }
                        }

                    }
                    post{
                        always{
                            script{
                                if(fileExists('reports/sonar-report.json')){
                                    stash includes: "reports/sonar-report.json", name: 'SONAR_REPORT'
                                    archiveArtifacts allowEmptyArchive: true, artifacts: 'reports/sonar-report.json'
                                    recordIssues(tools: [sonarQube(pattern: 'reports/sonar-report.json')])
                                }
                            }
                        }
                    }
                }
            }
        }
        stage('Build Documentation') {
            agent{
                dockerfile {
                    filename 'ci/dockerfiles/conan/dockerfile'
                    additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                    label "linux"
                }
            }
            when{
                equals expected: true, actual: params.BUILD_DOCUMENTATION
                beforeAgent true
            }
            steps{
                sh(label: "Building Doxygen documentation",
                   script:'''cmake -B ./build/docs/
                             cmake --build ./build/docs/ --target documentation
                             '''
                )
            }
            post{
                success{
                    publishHTML(
                        [
                            allowMissing: false,
                            alwaysLinkToLastBuild: false,
                            keepAll: false,
                            reportDir: 'build/docs/docs/html/',
                            reportFiles: 'index.html',
                            reportName: 'Documentation',
                            reportTitles: '',
                            includes: '**/*',
                        ]
                    )
                }
            }
        }
        stage("Packaging"){
            when{
                equals expected: true, actual: params.PACKAGE
            }
            stages{
                stage('Package Source and Linux binary Packages') {
                    agent{
                        dockerfile {
                            filename 'ci/dockerfiles/conan/dockerfile'
                            additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                            label "linux"
                        }
                    }
                    stages{
                        stage("Config and create a release build"){
                            steps{
                                sh(label: "Creating release build",
                                    script: '''cmake -B build/release
                                               cmake --build build/release
                                    '''
                                )
                            }
                        }
                        stage("Packaging"){
                            parallel{
                                stage("Creating CPack sdist"){
                                    steps{
                                        sh(label: "Creating CPack sdist",
                                           script: '''mkdir -p dist
                                                      cd dist && cpack --config ../build/release/CPackSourceConfig.cmake -G ZIP
                                                      '''
                                        )
                                    }
                                }
                            }
                        }
                    }
                    post{
                        cleanup{
                            cleanWs(
                                deleteDirs: true,
                                patterns: [
                                    [pattern: 'build/', type: 'INCLUDE'],
                                    [pattern: 'dist/', type: 'INCLUDE'],
                                ]
                            )
                        }
                    }
                }
                stage("Python Packages"){
                    matrix{
                        axes{
                            axis {
                                name "PYTHON_VERSION"
                                values(
                                    "3.7",
                                    "3.8",
                                    "3.9-rc"
                                )
                            }
                        }
                        agent{
                            dockerfile {
                                filename 'ci/dockerfiles/python/linux/Dockerfile'
                                additionalBuildArgs "--build-arg USER_ID=\$(id -u) --build-arg GROUP_ID=\$(id -g) --build-arg PYTHON_VERSION=${PYTHON_VERSION}"
                                label "linux"
                            }
                        }
                        stages{
                            stage("Build Python package"){
                                steps{
                                    sh(
                                        label: "Building packages",
                                        script: '''python setup.py build bdist_wheel --dist-dir=./dist sdist --dist-dir=./dist
                                                   ls -laR ./dist/
                                        '''
                                    )
                                }
                            }
                            stage("Testing Python on wheel package"){
                                options {
                                    warnError('Python whl test failed')
                                }
                                steps{
                                    script{
                                        findFiles(glob: "dist/*.whl").each{
                                            sh(
                                                label:"Running ${it.path}",
                                                script: "tox --installpkg=${it.path} -e py --recreate -v",
                                            )
                                        }
                                    }
                                }
                            }
                            stage("Testing Python on sdist package"){
                                options {
                                    warnError('Python sdist test failed')
                                }
                                steps{
                                    script{
                                        findFiles(glob: "dist/*.tar.gz,dist/*.zip").each{
                                            sh(
                                                label:"Running ${it.path}",
                                                script: "tox --installpkg=${it.path} -e py --recreate -v",
                                            )
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}