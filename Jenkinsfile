pipeline {
    agent none
    stages {
        stage("Static Analysis"){
            parallel{
                stage("Clang Tidy"){
                    agent{
                        dockerfile {
                            filename 'ci/dockerfiles/conan/dockerfile'
                            additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                            label "linux"
                        }
                    }
                    steps{
                        sh "wget -nc https://raw.githubusercontent.com/llvm-mirror/clang-tools-extra/master/clang-tidy/tool/run-clang-tidy.py"
                        tee('logs/clang-tidy_debug.log') {
                            sh  '''cmake -B ./build/debug/ -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON
                                   python3 run-clang-tidy.py -clang-tidy-binary clang-tidy -p ./build/debug/
                                   '''
                        }
                    }
                    post{
                        always {
                            archiveArtifacts(
                                allowEmptyArchive: true,
                                artifacts: 'logs/clang-tidy_debug.log'
                            )
                            recordIssues(tools: [clangTidy(pattern: 'logs/clang-tidy_debug.log')])
                        }
                        cleanup{
                            cleanWs(
                                patterns: [
                                    [pattern: 'logs/clang-tidy_debug.log', type: 'INCLUDE'],
                                ]
                            )
                        }
                    }
                }
                stage("Cppcheck"){
                    agent{
                      dockerfile {
                        filename 'ci/dockerfiles/conan/dockerfile'
                        additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                        label "linux"
                      }
                    }
                    steps{
                        cmake arguments: '-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON ..', installation: 'InSearchPath', workingDir: 'build'
                        sh(
                            label: "Running Cppcheck",
                            script: 'mkdir -p logs && cppcheck --project=build/compile_commands.json --enable=all  -ibuild/_deps --xml 2>logs/cppcheck_debug.xml'
                        )
                    }
                    post{
                        always {
                            sh "ls -la logs"
                            archiveArtifacts(
                              allowEmptyArchive: true,
                              artifacts: 'logs/cppcheck_debug.xml'
                            )
                            recordIssues(tools: [cppCheck(pattern: 'logs/cppcheck_debug.xml')])
                        }
                        cleanup{
                            cleanWs(
                              patterns: [
                                [pattern: 'logs/cppcheck_debug.xml', type: 'INCLUDE'],
                              ]
                            )
                        }
                    }
                }
            }
        }
        stage("Run Tests"){
            parallel{
                stage("Run CTest"){
                    agent{
                        dockerfile {
                          filename 'ci/dockerfiles/conan/dockerfile'
                          additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                          label "linux"
                        }
                    }
                    options {
                      warnError('Unstable')
                    }
                    steps{
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
                        ctest(
                          arguments: "--output-on-failure --no-compress-output -T Test",
                          installation: 'InSearchPath',
                          workingDir: "build/debug"
                          )
                    }
                    post{
                        always{
                            archiveArtifacts "build/debug/Testing/**/Test.xml"
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
                    agent{
                        dockerfile {
                          filename 'ci/dockerfiles/conan/dockerfile'
                          additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                          label "linux"
                        }
                    }
                    options {
                      warnError('Unstable')
                    }
                    steps{
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
                        ctest arguments: "-T coverage",
                          installation: 'InSearchPath',
                          workingDir: 'build/debug'
                    }
                    post{
                        always{
                          sh "mkdir -p reports/coverage"

                          sh(
                            label: "Generating coverage report in Coberatura xml file format",
                            script: "gcovr -r ./ --xml -o reports/coverage/coverage.xml build/debug"

                          )
                          archiveArtifacts 'reports/coverage/coverage.xml'
                          publishCoverage(
                            adapters: [coberturaAdapter('reports/coverage/coverage.xml')],
                            sourceFileResolver: sourceFiles('STORE_LAST_BUILD'),
                            tag: "AllCoverage"
                            )
                          sh(
                              label: "Generating coverage report in html file format",
                              script: "gcovr -r ./ --html --html-details -o reports/coverage/coverage.html build/debug"
                           )


                          publishHTML([allowMissing: true, alwaysLinkToLastBuild: false, keepAll: false, reportDir: 'reports/coverage', reportFiles: 'coverage.html', reportName: 'Coverage HTML Report', reportTitles: ''])

                        }
                  }
                }
                stage("CTest: MemCheck"){
                    agent{
                        dockerfile {
                              filename 'ci/dockerfiles/conan/dockerfile'
                              additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                              label "linux"
                        }
                    }
                    options {
                      warnError('Unstable')
                    }
                  steps{
                    script{

                      def cores = sh(
                        label: 'looking up number of cores',
                        returnStdout: true,
                        script: 'grep -c ^processor /proc/cpuinfo'
                      ).trim()
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
                      ctest(
                        arguments: "-T memcheck -j${cores}",
                        installation: 'InSearchPath',
                        workingDir: 'build/debug'
                        )
                    }
                  }
                  post{
                    always{
                        archiveArtifacts "build/debug/Testing/**/DynamicAnalysis.xml"
                    }
                  }
              }
                stage("Running Pytest"){
                    agent{
                        dockerfile {
                          filename 'ci/dockerfiles/conan/dockerfile'
                          additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                          label "linux"
                        }
                    }
                    options {
                      warnError('Unstable')
                    }
                  steps{
                    catchError(buildResult: 'UNSTABLE', message: 'Did not pass all Pytest tests', stageResult: 'UNSTABLE') {
                        sh(
                            label: "Running pytest",
                            script: ". ${WORKSPACE}/venv/bin/activate && coverage run --parallel-mode --branch --source=src/applications/pyvisvid/pyvisvid -m pytest --junitxml=${WORKSPACE}/reports/pytest/junit-pytest.xml"
                        )
                    }
                  }
                  post{
                    always{
                        junit "reports/pytest/junit-pytest.xml"
                    }
                  }
              }
                stage("Run MyPy Static Analysis") {
                    agent{
                        dockerfile {
                          filename 'ci/dockerfiles/conan/dockerfile'
                          additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                          label "linux"
                        }
                    }
                    options {
                      warnError('Unstable')
                    }
                  steps{
                      catchError(buildResult: 'SUCCESS', message: 'MyPy found issues', stageResult: 'UNSTABLE') {
                          tee("${WORKSPACE}/logs/mypy.log"){
                              sh(
                                label: "Running MyPy",
                                script: ". ${WORKSPACE}/venv/bin/activate && tox -e mypy -- --html-report ${WORKSPACE}/reports/mypy/html"
                                )
                            }
                      }
                  }
                  post {
                      always {
                          recordIssues(tools: [myPy(name: 'MyPy', pattern: 'logs/mypy.log')])
                          publishHTML([allowMissing: true, alwaysLinkToLastBuild: false, keepAll: false, reportDir: "reports/mypy/html/", reportFiles: 'index.html', reportName: 'MyPy HTML Report', reportTitles: ''])
                      }
                  }
              }
                stage("Run Flake8 Static Analysis") {
                    agent{
                        dockerfile {
                          filename 'ci/dockerfiles/conan/dockerfile'
                          additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                          label "linux"
                        }
                    }
                    options {
                      warnError('Unstable')
                    }
                  steps{
                      catchError(buildResult: 'SUCCESS', message: 'Flake8 found issues', stageResult: 'UNSTABLE') {

                          sh(
                              label: "Running Flake8",
                              script: """. ${WORKSPACE}/venv/bin/activate
                                            tox -e flake8 -- --tee --output-file=${WORKSPACE}/logs/flake8.log
                                            """
                          )
                      }
                  }
                  post {
                      always {
                          archiveArtifacts 'logs/flake8.log'
                          recordIssues(tools: [flake8(pattern: 'logs/flake8.log')])
                      }
                      unstable{
                        echo "I'm unstable"
                      }
                      cleanup{
                          cleanWs(patterns: [[pattern: 'logs/flake8.log', type: 'INCLUDE']])
                      }
                  }
              }
                stage("Running Tox"){
                    agent{
                        dockerfile {
                          filename 'ci/dockerfiles/conan/dockerfile'
                          additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                          label "linux"
                        }
                    }
                    options {
                      warnError('Unstable')
                    }
                    steps{
                        catchError(buildResult: 'UNSTABLE', message: 'Tox failed') {
                            sh(
                                label: "Running Tox",
                                script: ". ${WORKSPACE}/venv/bin/activate && tox --workdir ${WORKSPACE}/tox -vv -e py"
                            )
                        }
                    }
                }
            }
        }
        stage('Package') {
            parallel{
              stage("CPack Packages"){
                  stages{
                    stage("CPack Source Package"){
                        agent{
                            dockerfile {
                              filename 'ci/dockerfiles/conan/dockerfile'
                              additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                              label "linux"
                            }
                        }
                      steps {
                          cmakeBuild(
                                  buildDir: 'build/release',
                                  buildType: 'Release',
                                  cleanBuild: true,
                                  installation: 'InSearchPath',
                                  steps: []
                                )
                        script{
                            findFiles(glob: '**/CPackSourceConfig.cmake').each{
                                echo "found ${it.path}"
                            }
                        }
                        cpack arguments: "--config ${WORKSPACE}/build/release/CPackSourceConfig.cmake  -G ZIP", installation: 'InSearchPath', workingDir: 'dist'
                        archiveArtifacts(artifacts: 'dist/*Source.zip', fingerprint: true, onlyIfSuccessful: true)

                      }
                    }
                  }
              }
              stage("Python Packages"){
                  stages{
                        stage("Building Python Packages"){
                            agent{
                                    dockerfile {
                                      filename 'ci/dockerfiles/conan/dockerfile'
                                      additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                                      label "linux"
                                    }
                            }
                            steps{
                                sh(
                                    label: "Running Python setup script to build wheel and sdist",
                                    script: "python3 setup.py build --build-temp=pyvisvid/build/ bdist_wheel --dist-dir=pyvisvid/dist sdist --dist-dir=pyvisvid/dist"
                                )
                            }
                        }
                        stage("Testing Python Packages"){
                            agent{
                                dockerfile {
                                  filename 'ci/dockerfiles/conan/dockerfile'
                                  additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                                  label "linux"
                                }
                            }
                            steps{
                                script{
                                    def python_packages = findFiles glob: "pyvisvid/dist/*.zip,pyvisvid/dist/*.whl,pyvisvid/dist/*.tar.gz"
                                    python_packages.each{
                                        sh(
                                            label: "Running Tox with ${it}",
                                            script: """. ${WORKSPACE}/venv/bin/activate
                                                       tox --workdir ${WORKSPACE}/tox --installpkg $WORKSPACE/${it} -vv -e py
                                                       """
                                        )
                                    }
                                }
                            }
                        }
                    }
                    post{
                        success{
                          archiveArtifacts(artifacts: 'pyvisvid/dist/*.zip,pyvisvid/dist/*.whl,pyvisvid/dist/*.tar.gz', fingerprint: true, onlyIfSuccessful: true)
                        }
                    }
                }
          }
        }
    }
}