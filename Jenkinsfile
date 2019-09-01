pipeline {
    agent none
//   agent {
//         dockerfile {
//           filename 'scm/ci/dockerfiles/jenkins-main'
//           additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
//         }
//
//   }
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
        stage("Create Release Build with Conan"){
            agent {
                dockerfile {
                    dir 'scm'
                    filename 'ci/dockerfiles/conan/dockerfile'
                    additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                }
            }
            steps{
                dir("build/conan"){
                    sh "conan install ${WORKSPACE}/scm/ --profile x64"
                }
            }
            post{
                cleanup{
                    deleteDir()
                }
            }

        }
        stage("Create Release Build"){
          agent {
                dockerfile {
                  filename 'scm/ci/dockerfiles/jenkins-main'
                  additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                }

          }
          steps {
            tee('logs/gcc_release.log') {
              cmakeBuild(
                buildDir: 'build/release', 
                buildType: 'Release', 
                cleanBuild: true, 
                cmakeArgs: '\
-DVISVID_BUILDDOCS:BOOL=ON \
-DCMAKE_C_FLAGS="-Wall -Wextra"',
                installation: 'InSearchPath',
                sourceDir: 'scm',
                steps: [[withCmake: true]]
              )
            }
            recordIssues(
                qualityGates: [[threshold: 5, type: 'TOTAL', unstable: true]],
                tools: [gcc4(pattern: 'logs/gcc_*.log')]
                )
          }
          post{
            always{
                stash includes: "build/release/", name: 'RELEASE_BUILD_FILES'
            }
            cleanup{
                deleteDir()
//                   cleanWs(
//                       disableDeferredWipeout: true,
//                       patterns: [
//                           [pattern: "build/release", type: 'INCLUDE'],
//                           ],
//                       deleteDirs: true
//                   )
            }
          }
        }
        stage("Create Debug Build"){
          agent {
                dockerfile {
                  filename 'scm/ci/dockerfiles/jenkins-main'
                  additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                }

          }
          steps {
            tee('logs/gcc_debug.log') {

              cmakeBuild(
                buildDir: 'build/debug',
                buildType: 'Debug',
                cleanBuild: true,
                installation: 'InSearchPath',
                cmakeArgs: '\
-DCTEST_DROP_LOCATION=$WORKSPACE/reports/ctest \
-DCMAKE_C_FLAGS_DEBUG="-fprofile-arcs -ftest-coverage" \
-DCMAKE_EXE_LINKER_FLAGS="-fprofile-arcs -ftest-coverage" \
-DCMAKE_C_FLAGS="-Wall -Wextra" \
-DVALGRIND_COMMAND_OPTIONS="--xml=yes --xml-file=mem-%p.memcheck" \
-Dlibvisvid_TESTS:BOOL=ON ',
                sourceDir: 'scm',
                steps: [
                  [args: '--target test-visvid', withCmake: true],
                  [args: '--target test-visvid-internal', withCmake: true],
                ]
              )
            }
          }
          post{
            always{
              stash includes: "build/debug/**", name: 'DEBUG_BUILD_FILES'
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
                deleteDir()
//                 cleanWs(
//                     disableDeferredWipeout: true,
//                     deleteDirs: true,
//                     patterns: [
//                         [pattern: "build/debug", type: 'INCLUDE'],
// //                         [pattern: "build/debug/**/*.memcheck", type: 'INCLUDE'],
//                         ]
//                 )
            }
          }
        }
        stage("Building Python Extension"){
            agent {
                dockerfile {
                  filename 'scm/ci/dockerfiles/jenkins-main'
                  additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                }

            }
            steps{
                dir("scm"){
                    sh(
                        label: "Running Python setup script to build extension inplace",
                        script: "python3 setup.py build --build-temp=../pyvisvid/build  build_ext --inplace"
                        )
                }
            }
            post{
                success{
                    stash includes: "pyvisvid/build/** ", name: 'PYTHON_BUILD_FILES'
                }
                failure{
                    deleteDir()
                }
                cleanup{
                    deleteDir()
//                     cleanWs(
//                         disableDeferredWipeout: true,
//                         patterns: [
//                             [pattern: "pyvisvid", type: 'INCLUDE'],
//                             ],
//                         deleteDirs: true
//                     )
                }
            }
        }
        stage('Documentation') {
              agent {
                    dockerfile {
                      filename 'scm/ci/dockerfiles/jenkins-main'
                      additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                    }

              }
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
                cleanup{
                    deleteDir()
//                     cleanWs(
//                         patterns: [
//                             [pattern: "dist", type: 'INCLUDE'],
//                             [pattern: "build", type: 'INCLUDE']
//                             ],
//                         deleteDirs: true
//                         )
                }
              }
        }
        
      }
    }
    stage("Static Analysis"){

      parallel{
        stage("Clang Tidy"){
            agent {
                dockerfile {
                  filename 'scm/ci/dockerfiles/jenkins-main'
                  additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                }
            }
            options{
                timeout(5)
            }
            steps{
                sh "wget -nc https://raw.githubusercontent.com/llvm-mirror/clang-tools-extra/master/clang-tidy/tool/run-clang-tidy.py"
                cmake arguments: '-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON ../scm', installation: 'InSearchPath', workingDir: 'build'
                tee("logs/clang-tidy_debug.log") {
                  sh  "python run-clang-tidy.py -clang-tidy-binary clang-tidy -p ./build/"
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
                    deleteDir()
//                     cleanWs(
//                       patterns: [
//                         [pattern: 'logs/clang-tidy_debug.log', type: 'INCLUDE'],
//                       ]
//                     )
                }

            }
        }
        stage("Cppcheck"){
            agent {
                dockerfile {
                  filename 'scm/ci/dockerfiles/jenkins-main'
                  additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                }

            }
              options{
                timeout(5)
              }
              steps{
                // TODO setup supressing files for 3rd party, esp catch2
//                     unstash "DEBUG_BUILD_FILES"
                  cmake arguments: '-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON ../scm', installation: 'InSearchPath', workingDir: 'build'
                  sh(
                    label: "Running Cppcheck",
                    script: "mkdir -p logs && cppcheck --project=build/compile_commands.json --enable=all  --suppress='*:${WORKSPACE}/build/_deps/*' --xml 2>logs/cppcheck_debug.xml"
                    )
              }
              post{
                always {
                    sh "ls -la logs"
                    archiveArtifacts(
                      allowEmptyArchive: true,
                      artifacts: 'logs/cppcheck_debug.log'
                    )
                    recordIssues(tools: [cppCheck(pattern: 'logs/cppcheck_debug.xml')])
                }
                cleanup{
                    deleteDir()
//                     cleanWs(
//                       patterns: [
//                         [pattern: 'logs/cppcheck_debug.log', type: 'INCLUDE'],
//                       ]
//                     )
                }

              }
        }
      }
    }
    stage('Test') {
//         agent {
//             dockerfile {
//               filename 'scm/ci/dockerfiles/jenkins-main'
//               additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
//             }

//         }
        stages{
//             stage("Setting Up Python Test Environment"){
//                 steps{
//                     unstash "PYTHON_BUILD_FILES"
//                     sh(
//                       label: "Install virtual env",
//                       script: "python3 -m venv venv"
//                       )
//
//                     sh(
//                       label: "Upgrade pip",
//                       script:""". ./venv/bin/activate
// python -m pip install pip --upgrade
// """
//                     )
//
//                     sh(
//                       label: "Installing Python Testing Packages",
//                       script: """. ./venv/bin/activate
// pip install pytest "tox<3.10" mypy coverage lxml"""
//                     )
//                     dir("scm"){
//                       sh(
//                           label: "Installing Current Python Package to Virtual Environment in Development Mode",
//                           script: """. ${WORKSPACE}/venv/bin/activate
//   python setup.py build
//   pip install -e ."""
//                       )
//                     }
//                   }
//                   post{
//                       failure{
//                           deleteDir()
//                       }
//                   }
//             }
            stage("Run Tests"){
              parallel{
//
                stage("Run CTest"){
                    agent {
                        dockerfile {
                          filename 'scm/ci/dockerfiles/jenkins-main'
                          additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
                        }

                    }
//                     options {
//                       lock(label: 'Docker')
//                     }
                    steps{
                        unstash "DEBUG_BUILD_FILES"
//                         ctest(
//                           arguments: "--output-on-failure --no-compress-output -T Test",
//                           installation: 'InSearchPath',
//                           workingDir: 'build/debug'
//                           )
                    }
                    post{
                        cleanup{
                            deleteDir()
                        }
                    }
                }
//                 stage("CTest: Coverage"){
//                     agent {
//                             dockerfile {
//                               filename 'scm/ci/dockerfiles/jenkins-main'
//                               additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
//                             }
//
//                     }
//                     options {
//                         lock(label: 'Docker')
//                     }
//                     steps{
//                         ctest arguments: "-T coverage",
//                           installation: 'InSearchPath',
//                           workingDir: 'build/debug'
//                     }
//                     post{
//                         always{
//                           sh "mkdir -p reports/coverage"
//
//                           sh(
//                             label: "Generating coverage report in Coberatura xml file format",
//                             script: "gcovr -r ./scm --xml -o reports/coverage/coverage.xml build/debug"
//
//                           )
//                           archiveArtifacts 'reports/coverage/coverage.xml'
//                           publishCoverage(
//                             adapters: [coberturaAdapter('reports/coverage/coverage.xml')],
//                             sourceFileResolver: sourceFiles('STORE_LAST_BUILD'),
//                             tag: "AllCoverage"
//                             )
//                           //////////////////////////////////////////
//                           sh(
//                               label: "Generating coverage report in html file format",
//                               script: "gcovr -r ./scm --html --html-details -o reports/coverage/coverage.html build/debug"
//                            )
//
//
//                           publishHTML([allowMissing: true, alwaysLinkToLastBuild: false, keepAll: false, reportDir: 'reports/coverage', reportFiles: 'coverage.html', reportName: 'Coverage HTML Report', reportTitles: ''])
//
//                         }
//                   }
//                 }
//                 stage("CTest: MemCheck"){
//                     agent {
//                         dockerfile {
//                           filename 'scm/ci/dockerfiles/jenkins-main'
//                           additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
//                         }
//                     }
//                     options {
//                         lock(label: 'Docker')
//                     }
//                   steps{
//                     script{
//
//                       def cores = sh(
//                         label: 'looking up number of cores',
//                         returnStdout: true,
//                         script: 'grep -c ^processor /proc/cpuinfo'
//                       ).trim()
//
//                       ctest(
//                         arguments: "-T memcheck -j${cores}",
//                         installation: 'InSearchPath',
//                         workingDir: 'build/debug'
//                         )
//                     }
//                   }
//                 }
//                 stage("Running Pytest"){
//                     agent {
//                         dockerfile {
//                           filename 'scm/ci/dockerfiles/jenkins-main'
//                           additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
//                         }
//
//                     }
//                     options {
//                         lock(label: 'Docker')
//                     }
//                   steps{
//                     dir("scm"){
//                         catchError(buildResult: 'UNSTABLE', message: 'Did not pass all Pytest tests', stageResult: 'UNSTABLE') {
//                             sh(
//                                 label: "Running pytest",
//                                 script: ". ${WORKSPACE}/venv/bin/activate && coverage run --parallel-mode --branch --source=src/applications/pyvisvid/pyvisvid -m pytest --junitxml=${WORKSPACE}/reports/pytest/junit-pytest.xml"
//                             )
//                         }
//                     }
//                   }
//                   post{
//                     always{
//                         junit "reports/pytest/junit-pytest.xml"
//                     }
//                   }
//               }
//               stage("Run MyPy Static Analysis") {
//                     agent {
//                           dockerfile {
//                             filename 'scm/ci/dockerfiles/jenkins-main'
//                             additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
//                           }
//
//                     }
//                     options {
//                         lock(label: 'Docker')
//                     }
//                   steps{
//                       dir("scm"){
//                           catchError(buildResult: 'SUCCESS', message: 'MyPy found issues', stageResult: 'UNSTABLE') {
//                               tee("${WORKSPACE}/logs/mypy.log"){
//                                   sh(
//                                     label: "Running MyPy",
//                                     script: ". ${WORKSPACE}/venv/bin/activate && tox -e mypy -- --html-report ${WORKSPACE}/reports/mypy/html"
//                                     )
//                                 }
//                           }
//
//                       }
//                   }
//                   post {
//                       always {
//                           recordIssues(tools: [myPy(name: 'MyPy', pattern: 'logs/mypy.log')])
//                           publishHTML([allowMissing: true, alwaysLinkToLastBuild: false, keepAll: false, reportDir: "reports/mypy/html/", reportFiles: 'index.html', reportName: 'MyPy HTML Report', reportTitles: ''])
//                       }
//                   }
//               }
//               stage("Run Flake8 Static Analysis") {
//                     agent {
//                           dockerfile {
//                             filename 'scm/ci/dockerfiles/jenkins-main'
//                             additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
//                           }
//
//                     }
//                     options {
//                         lock(label: 'Docker')
//                     }
//                   steps{
//                       dir("scm"){
//                           catchError(buildResult: 'SUCCESS', message: 'Flake8 found issues', stageResult: 'UNSTABLE') {
//
//                               sh(
//                                   label: "Running Flake8",
//                                   script: """. ${WORKSPACE}/venv/bin/activate
// tox -e flake8 -- --tee --output-file=${WORKSPACE}/logs/flake8.log
// """
//
//                               )
//                           }
//                       }
//                   }
//                   post {
//                       always {
//                           archiveArtifacts 'logs/flake8.log'
//                           recordIssues(tools: [flake8(pattern: 'logs/flake8.log')])
//                       }
//                       unstable{
//                         echo "I'm unstable"
//                       }
//                       cleanup{
//                           cleanWs(patterns: [[pattern: 'logs/flake8.log', type: 'INCLUDE']])
//                       }
//                   }
//               }
//               stage("Running Tox"){
//                     agent {
//                             dockerfile {
//                               filename 'scm/ci/dockerfiles/jenkins-main'
//                               additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
//                             }
//                     }
//                     options {
//                         lock(label: 'Docker')
//                     }
//                   steps{
//                       catchError(buildResult: 'UNSTABLE', message: 'Tox failed') {
//                           sh(
//                             label: "Running Tox",
//                             script: ". ${WORKSPACE}/venv/bin/activate && cd scm && tox --workdir ${WORKSPACE}/tox -vv -e py"
//                           )
//                       }
//
//                   }
//               }
//               }
            }
          }
//       post{
//         always{
//             ctest arguments: "-T Submit", installation: 'InSearchPath', workingDir: 'build/debug'
//             archiveArtifacts allowEmptyArchive: true, artifacts:"reports/ctest/*.*"
//             xunit testTimeMargin: '3000',
//                 thresholdMode: 1,
//                 thresholds: [
//                   failed(),
//                   skipped()
//                   ],
//                 tools: [
//                   CTest(
//                     deleteOutputFiles: true,
//                     failIfNotNew: true,
//                     pattern: "reports/ctest/*.xml",
//                     skipNoTestFiles: true,
//                     stopProcessingIfError: true
//                     )
//                   ]
//             dir("scm"){
//                 sh(
//                     label: "Combining coverage data",
//                     script: """
// . ${WORKSPACE}/venv/bin/activate
// coverage combine
// coverage xml -o ${WORKSPACE}/reports/python/coverage.xml
// coverage html -d ${WORKSPACE}/reports/python/coverage
// """
//                 )
//             }
//             publishHTML([allowMissing: true, alwaysLinkToLastBuild: false, keepAll: false, reportDir: "reports/python/coverage", reportFiles: 'index.html', reportName: 'Python Coverage', reportTitles: ''])
//             publishCoverage(
//                 adapters: [
//                     coberturaAdapter('reports/python/coverage.xml')
//                     ],
//                 sourceFileResolver: sourceFiles('STORE_ALL_BUILD'),
//                 tag: "AllCoverage"
//
//             )
//         }
//         cleanup{
//             cleanWs(
//                 deleteDirs: true,
//                 patterns: [
//                     [pattern: 'reports/ctest', type: 'INCLUDE']
//                     ]
//                 )
//         }
      }
    }
//     stage('Package') {
//
//       parallel{
//           stage("CPack Packages"){
//             agent {
//                   dockerfile {
//                     filename 'scm/ci/dockerfiles/jenkins-main'
//                     additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
//                   }
//
//             }
//               stages{
//                 stage("CPack Source Package"){
//                   steps {
//                     cpack arguments: "--config ${WORKSPACE}/build/release/CPackSourceConfig.cmake  -G ZIP", installation: 'InSearchPath', workingDir: 'dist'
//                     archiveArtifacts(artifacts: 'dist/*Source.zip', fingerprint: true, onlyIfSuccessful: true)
//
//                   }
//                 }
//               }
//           }
//           stage("Python Packages"){
//                 agent {
//                       dockerfile {
//                         filename 'scm/ci/dockerfiles/jenkins-main'
//                         additionalBuildArgs '--build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g)'
//                       }
//
//                 }
//               stages{
//                     stage("Building Python Packages"){
//                         steps{
//                             sh "mkdir -p scm && ls pyvisvid/build"
//                             dir("scm"){
//                                 sh(
//                                     label: "Running Python setup script to build wheel and sdist",
//                                     script: "python3 setup.py build --build-temp=../pyvisvid/build/ bdist_wheel --dist-dir=${WORKSPACE}/pyvisvid/dist sdist --dist-dir=${WORKSPACE}/pyvisvid/dist"
//                                 )
//                             }
//                         }
//                     }
//                     stage("Testing Python Packages"){
//                         steps{
//                             script{
//                                 def python_packages = findFiles glob: "pyvisvid/dist/*.zip,pyvisvid/dist/*.whl,pyvisvid/dist/*.tar.gz"
//                                 python_packages.each{
//                                     sh(
//                                         label: "Running Tox with ${it}",
//                                         script: """. ${WORKSPACE}/venv/bin/activate
// cd scm && tox --workdir ${WORKSPACE}/tox --installpkg $WORKSPACE/${it} -vv -e py"""
//                                     )
//                                 }
//                             }
//                         }
//                     }
//                 }
//                 post{
//                     success{
//                       archiveArtifacts(artifacts: 'pyvisvid/dist/*.zip,pyvisvid/dist/*.whl,pyvisvid/dist/*.tar.gz', fingerprint: true, onlyIfSuccessful: true)
//                     }
//                 }
//             }
//       }
//     }
  }
//   post {
//     cleanup{
//         cleanWs(
//             deleteDirs: true,
//             patterns: [
//             [pattern: 'build', type: 'INCLUDE'],
//             [pattern: 'dist', type: 'INCLUDE'],
//             [pattern: 'generatedJUnitFiles', type: 'INCLUDE'],
//             [pattern: 'scm', type: 'INCLUDE'],
//             [pattern: 'reports', type: 'INCLUDE'],
//             [pattern: 'logs', type: 'INCLUDE'],
//             [pattern: 'tox', type: 'INCLUDE'],
//             [pattern: 'pyvisvid', type: 'INCLUDE'],
//             [pattern: '*tmp', type: 'INCLUDE'],
//             [pattern: 'venv', type: 'INCLUDE'],
//             [pattern: 'testresults', type: 'INCLUDE']
//             ])
//     }
//   }
}