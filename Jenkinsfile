pipeline {
    agent none
    parameters{
        booleanParam(name: "TEST_STATIC_ANALYSIS", defaultValue: false, description: "Run Static Analysis checks")
        booleanParam(name: "TEST_CTEST", defaultValue: false, description: "Run ctest checks")
        booleanParam(name: "PACKAGE", defaultValue: false, description: "Create distribution packages")
    }
    stages {
        stage("Checks"){
            stages{
                stage("Static Analysis"){
                    when{
                        equals expected: true, actual: params.TEST_STATIC_ANALYSIS
                    }
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
                stage("Run Tests"){
                    agent{
                        dockerfile {
                            filename 'ci/dockerfiles/conan/dockerfile'
                            additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                            label "linux"
                        }
                    }
                    when{
                        equals expected: true, actual: params.TEST_CTEST
                        beforeAgent true
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
                        stage("Running Checks"){
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
                }
            }
        }
        stage('Package Source and Linux binary Packages') {
            agent{
                dockerfile {
                    filename 'ci/dockerfiles/conan/dockerfile'
                    additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                    label "linux"
                }
            }
            when{
                equals expected: true, actual: params.PACKAGE
                beforeAgent true
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
    }
}