pipeline {
  agent {
    dockerfile {
      filename 'scm/ci/dockerfiles/jenkins-main'
      additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
    }
    
  }
  options {
    timeout(30)
    checkoutToSubdirectory 'scm'
    buildDiscarder(
        logRotator(
            artifactDaysToKeepStr: '10',
            artifactNumToKeepStr: '10',
            daysToKeepStr: '20',
            numToKeepStr: '20'
        )
    )
  }
  stages {
    stage('Build') {
      parallel{
        stage("Create Release Build"){
          steps {
            tee('logs/gcc_release.log') {
              cmakeBuild(
                buildDir: 'build/release', 
                buildType: 'Release', 
                cleanBuild: true, 
                cmakeArgs: '-DVISVID_BUILDDOCS:BOOL=ON -DCMAKE_C_FLAGS="-Wall"', 
                installation: 'InSearchPath',
                sourceDir: 'scm',
                steps: [[withCmake: true]]
              )
            }
          }
          post{
            success{
              stash includes: "build/release/", name: 'RELEASE_BUILD_FILES'
              recordIssues(tools: [gcc4(pattern: 'logs/gcc_*.log')])
            }
          }
        }
        stage("Create Debug Build"){       
          steps {
            tee('logs/gcc_debug.log') {

              cmakeBuild(
                buildDir: 'build/debug',
                buildType: 'Debug', 
                cleanBuild: true, 
                installation: 'InSearchPath', 
                cmakeArgs: '-DCTEST_DROP_LOCATION=$WORKSPACE/reports/ctest -DCMAKE_C_FLAGS_DEBUG="-fprofile-arcs -ftest-coverage" -DCMAKE_EXE_LINKER_FLAGS="-fprofile-arcs -ftest-coverage" -DCMAKE_C_FLAGS="-Wall" -DVALGRIND_COMMAND_OPTIONS="--xml=yes --xml-file=mem-%p.memcheck" -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON',
                sourceDir: 'scm',
                steps: [
                [args: '--target test-visvid', withCmake: true],
                [args: '--target test-visvid-internal', withCmake: true],
                ]
              )
            }
          }
          post{
            success{
              stash includes: "build/debug/", name: 'DEBUG_BUILD_FILES'
              publishValgrind (
                        failBuildOnInvalidReports: false,
                        failBuildOnMissingReports: false,
                        failThresholdDefinitelyLost: '',
                        failThresholdInvalidReadWrite: '',
                        failThresholdTotal: '',
                        pattern: 'build/debug/**/*.memcheck',
                        publishResultsForAbortedBuilds: false,
                        publishResultsForFailedBuilds: false,
                        sourceSubstitutionPaths: '',
                        unstableThresholdDefinitelyLost: '',
                        unstableThresholdInvalidReadWrite: '',
                        unstableThresholdTotal: ''
                      )

            }
            cleanup{
                cleanWs(patterns: [[pattern: "build/debug/**/*.memcheck", type: 'INCLUDE']])
            }
          }
        }
        stage("Building Python Extension"){
            steps{
                dir("scm"){
                    sh(
                        label: "Running Python setup script to build extension inplace",
                        script: "python3 setup.py build --build-temp=${WORKSPACE}/pyvisvid/build  build_ext --inplace"
                    )
                }
            }
        }
        stage('Documentation') {
              steps {
                cmakeBuild(
                  buildDir: 'build/docs',
                  buildType: 'Release',
                  cleanBuild: true,
                  cmakeArgs: '',
                  installation: 'InSearchPath',
                  sourceDir: 'scm',
                  steps: [[args: '--target documentation', withCmake: true]]
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
                  zip(zipFile: 'dist/visvid_documentation.zip', archive: true, dir: 'build/docs/docs/html')
                  stash includes: "build/docs/docs/html/**", name: 'DOCS_ARCHIVE'
                }
              }
        }
        
      }
    }
    stage("Static Analysis"){
      parallel{
        stage("Clang Tidy"){
          options{
            timeout(5)
          }
          steps{
            sh "wget -nc https://raw.githubusercontent.com/llvm-mirror/clang-tools-extra/master/clang-tidy/tool/run-clang-tidy.py"
            tee('logs/clang-tidy_debug.log') {
              sh  "python run-clang-tidy.py -clang-tidy-binary clang-tidy-9 -p ./build/debug/"
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
                    [pattern: 'build/clang-tidy', type: 'INCLUDE'],
                    [pattern: 'logs/clang-tidy_debug.log', type: 'INCLUDE'],
                  ]
                )
            }
            
          }
        }
      }
    }
    stage('Test') {
        stages{
            stage("Setting Up Python Test Environment"){
                steps{
                    sh(
                      label: "Install virtual env",
                      script: "python3 -m venv venv"
                      )

                    sh(
                      label: "Upgrade pip",
                      script:""". ./venv/bin/activate
python -m pip install pip --upgrade
"""
                    )

                    sh(
                      label: "Installing Python Testing Packages",
                      script: """. ./venv/bin/activate
pip install pytest "tox<3.10" flake8 mypy coverage lxml"""
                    )
                    dir("scm"){
                      sh(
                          label: "Installing Current Python Package to Virtual Environment in Development Mode",
                          script: """. ${WORKSPACE}/venv/bin/activate
  pip install -e ."""
                      )
                    }
                  }
            }
            stage("Run Tests"){
              parallel{

                stage("Run CTest"){
                  steps{
                    unstash "DEBUG_BUILD_FILES"
                    ctest(
                      arguments: "--output-on-failure --no-compress-output -T Test",
                      installation: 'InSearchPath',
                      workingDir: 'build/debug'
                      )
                  }
                }
                stage("CTest: Coverage"){
                  steps{
                    ctest arguments: "-T coverage",
                      installation: 'InSearchPath',
                      workingDir: 'build/debug'
                  }
                  post{
                    always{
                      sh(
                        label: "Generating coverage report in Coberatura xml file format",
                        script: "mkdir -p reports/coverage && gcovr -r ./scm --xml -o reports/coverage/coverage.xml build/debug"
                      )

                      sh(
                          label: "Generating coverage report in html file format",
                          script: "gcovr -r ./scm --html --html-details -o reports/coverage/coverage.html build/debug"
                       )
                      archiveArtifacts 'reports/coverage/coverage.xml'
                      publishCoverage(
                        adapters: [coberturaAdapter('reports/coverage/coverage.xml')],
                        sourceFileResolver: sourceFiles('STORE_LAST_BUILD'),
                        tag: "AllCoverage"
                        )

                      publishHTML([allowMissing: true, alwaysLinkToLastBuild: false, keepAll: false, reportDir: 'reports/coverage', reportFiles: 'coverage.html', reportName: 'Coverage HTML Report', reportTitles: ''])

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
                }
                stage("Running Pytest"){
                  steps{
                    dir("scm"){
                        catchError(buildResult: 'UNSTABLE', message: 'Did not pass all Pytest tests', stageResult: 'UNSTABLE') {
                            sh(
                                label: "Running pytest",
                                script: ". ${WORKSPACE}/venv/bin/activate && coverage run --parallel-mode --branch --source=examples/pyvisvid/pyvisvid -m pytest --junitxml=${WORKSPACE}/reports/pytest/junit-pytest.xml"
                            )
                        }
                    }
                  }
                  post{
                    always{
                        junit "reports/pytest/junit-pytest.xml"
                    }
                  }
              }
              stage("Run MyPy Static Analysis") {
                  steps{
                      dir("scm"){
                          catchError(buildResult: 'SUCCESS', message: 'MyPy found issues', stageResult: 'UNSTABLE') {
                              tee("${WORKSPACE}/logs/mypy.log"){
                                  sh(
                                    label: "Running MyPy",
                                    script: ". ${WORKSPACE}/venv/bin/activate && mypy examples/pyvisvid/pyvisvid  --cache-dir=${WORKSPACE}/mypy_cache --html-report ${WORKSPACE}/reports/mypy/html"
                                    )
                                }
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
                  steps{
                      dir("scm"){
                          catchError(buildResult: 'SUCCESS', message: 'Flake8 found issues', stageResult: 'UNSTABLE') {

                              sh(
                                  label: "Running Flake8",
                                  script: """. ${WORKSPACE}/venv/bin/activate
flake8 examples/pyvisvid/pyvisvid --tee --output-file=${WORKSPACE}/logs/flake8.log
"""

                              )
                          }
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

                  steps{
                      catchError(buildResult: 'UNSTABLE', message: 'Tox failed') {
                          sh(
                            label: "Running Tox",
                            script: ". ${WORKSPACE}/venv/bin/activate && cd scm && tox --workdir ${WORKSPACE}/tox -vv -e py"
                          )
                      }

                  }
              }
              }
            }
          }
      post{
        always{
            ctest arguments: "-T Submit", installation: 'InSearchPath', workingDir: 'build/debug'
            archiveArtifacts allowEmptyArchive: true, artifacts:"reports/ctest/*.*"
            xunit testTimeMargin: '3000',
                thresholdMode: 1,
                thresholds: [
                  failed(),
                  skipped()
                  ],
                tools: [
                  CTest(
                    deleteOutputFiles: true,
                    failIfNotNew: true,
                    pattern: "reports/ctest/*.xml",
                    skipNoTestFiles: true,
                    stopProcessingIfError: true
                    )
                  ]
            dir("scm"){
                sh(
                    label: "Combining coverage data",
                    script: """
. ${WORKSPACE}/venv/bin/activate
coverage combine
coverage xml -o ${WORKSPACE}/reports/python/coverage.xml
coverage html -d ${WORKSPACE}/reports/python/coverage
"""
                )
            }
            publishHTML([allowMissing: true, alwaysLinkToLastBuild: false, keepAll: false, reportDir: "reports/python/coverage", reportFiles: 'index.html', reportName: 'Python Coverage', reportTitles: ''])
            publishCoverage(
                adapters: [
                    coberturaAdapter('reports/python/coverage.xml')
                    ],
                sourceFileResolver: sourceFiles('STORE_ALL_BUILD'),
                tag: "AllCoverage"

            )
        }
        cleanup{
            cleanWs(
                deleteDirs: true,
                patterns: [
                    [pattern: 'reports/ctest', type: 'INCLUDE']
                    ]
                )
        }
      }
    }
    stage('Package') {
      parallel{
          stage("CPack Packages"){
              stages{
                stage("CPack Source Package"){
                  steps {
                    cpack arguments: "--config ${WORKSPACE}/build/release/CPackSourceConfig.cmake  -G ZIP", installation: 'InSearchPath', workingDir: 'dist'
                    archiveArtifacts(artifacts: 'dist/*Source.zip', fingerprint: true, onlyIfSuccessful: true)

                  }
                }
              }
          }
          stage("Python Packages"){
              stages{
                    stage("Building Python Packages"){
                        steps{
                            dir("scm"){
                                sh(
                                    label: "Running Python setup script to build wheel and sdist",
                                    script: "python3 setup.py  build --build-temp=${WORKSPACE}/pyvisvid/build/ bdist_wheel --dist-dir=${WORKSPACE}/pyvisvid/dist sdist --dist-dir=${WORKSPACE}/pyvisvid/dist"
                                )
                            }
                        }
                    }
                    stage("Testing Python Packages"){
                        steps{
                            script{
                                def python_packages = findFiles glob: "pyvisvid/dist/*.zip,pyvisvid/dist/*.whl,pyvisvid/dist/*.tar.gz"
                                python_packages.each{
                                    sh(
                                        label: "Running Tox with ${it}",
                                        script: """. ${WORKSPACE}/venv/bin/activate
cd scm && tox --workdir ${WORKSPACE}/tox --installpkg $WORKSPACE/${it} -vv -e py"""
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
  post {
    cleanup{
        cleanWs(
            deleteDirs: true,
            patterns: [
            [pattern: 'build', type: 'INCLUDE'], 
            [pattern: 'dist', type: 'INCLUDE'],
            [pattern: 'generatedJUnitFiles', type: 'INCLUDE'],
            [pattern: 'scm', type: 'INCLUDE'],
            [pattern: 'reports', type: 'INCLUDE'],
            [pattern: 'logs', type: 'INCLUDE'],
            [pattern: 'tox', type: 'INCLUDE'],
            [pattern: 'pyvisvid', type: 'INCLUDE'],
            [pattern: '*tmp', type: 'INCLUDE'],
            [pattern: 'venv', type: 'INCLUDE'],
            [pattern: 'testresults', type: 'INCLUDE']
            ])
    }
  }
}