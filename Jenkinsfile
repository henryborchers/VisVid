pipeline {
  agent {
    dockerfile {
      filename 'scm/Dockerfile'
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
                cmakeArgs: "-DCTEST_DROP_LOCATION=${WORKSPACE}/reports/ctest -DCMAKE_C_FLAGS_DEBUG=\"-fprofile-arcs -ftest-coverage\" -DCMAKE_EXE_LINKER_FLAGS=\"-fprofile-arcs -ftest-coverage\" -DCMAKE_C_FLAGS=\"-Wall\"",
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
            }
          }
        }
        stage('Documentation') {
              // steps {
              //   stage('Build') {
              steps {
//                unstash "RELEASE_BUILD_FILES"
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
                      reportDir: 'build/release/html/',
                      reportFiles: 'index.html',
                      reportName: 'Documentation',
                      reportTitles: '',
                      includes: '**/*',
                    ]
                  )
                  zip(zipFile: 'dist/visvid_documentation.zip', archive: true, dir: 'build/docs/html')
                  stash includes: "build/docs/html/**", name: 'DOCS_ARCHIVE'
                }
//                cleanup{
//                    cleanWs(patterns: [[pattern: "dist/visvid_documentation.zip", type: 'INCLUDE']])
//                }
              }
        }
        
      }
      post{
          success{
            recordIssues(tools: [gcc4(pattern: 'logs/gcc_*.log')])
          }
        }
    }
    stage('Test') {
      stages{
        
        stage("Run CTest"){
          steps{
            unstash "DEBUG_BUILD_FILES"
            ctest( 
              arguments: "--output-on-failure --no-compress-output -T Test", 
              installation: 'InSearchPath', 
              workingDir: 'build/debug'
              )

            // sh 'ctest -S build.cmake --verbose'

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
              dir("reports/coverage"){
                deleteDir()
                sh "ls"
              }
              sh "mkdir -p reports/coverage && gcovr -r . --xml -o reports/coverage/coverage.xml build/debug"
              sh "gcovr -r . --html --html-details -o reports/coverage/coverage.html build/debug"
              archiveArtifacts 'reports/coverage/coverage.xml'
              publishCoverage adapters: [coberturaAdapter('reports/coverage/coverage.xml')], sourceFileResolver: sourceFiles('STORE_LAST_BUILD')
              publishHTML([allowMissing: true, alwaysLinkToLastBuild: false, keepAll: false, reportDir: 'reports/coverage', reportFiles: 'coverage.html', reportName: 'Coverage HTML Report', reportTitles: ''])

            }
            cleanup{
              dir("reports/coverage"){
                deleteDir()
              }
            //   // cleanWs(patterns: [[pattern: "${WORKSPACE}/reports/coverage/coverage.xml", type: 'INCLUDE']])
            }
          }
        }
        stage("CTest: MemCheck"){
          steps{
            ctest(
              arguments: "-T memcheck", 
              installation: 'InSearchPath', 
              workingDir: 'build/debug'
              )
          }
        }
      }
      post{
        always{
            ctest arguments: "-T Submit", installation: 'InSearchPath', workingDir: 'build/debug'
            sh "ls reports/ctest"
            archiveArtifacts "reports/ctest/*.*"
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

//     stage('Package') {
//       steps {
//         sh '''cd build
// cpack -G ZIP'''
//         archiveArtifacts(artifacts: 'build/*.zip', fingerprint: true, onlyIfSuccessful: true)
//       }
//     }
  // } 
    }
    post {
        failure {
          sh "tree > tree.log"
          archiveArtifacts(
            artifacts: "tree.log"
            )

      //     step([$class: 'XUnitBuilder',
      //       thresholds: [
      //             [$class: 'SkippedThreshold', failureThreshold: '0'],
      //             [$class: 'FailedThreshold', failureThreshold: '0']],
      //         tools: [[$class: 'CTestType', pattern: 'build/Testing/**/*.xml']]])
      //       echo 'cleaning up'
      //       deleteDir()
        }
        cleanup{
          cleanWs(
            deleteDirs: true,
            patterns: [
            [pattern: 'build', type: 'INCLUDE'], 
            [pattern: 'dist', type: 'INCLUDE'],
            [pattern: 'generatedJUnitFiles', type: 'INCLUDE'],
            [pattern: 'reports', type: 'INCLUDE'],
            [pattern: 'logs', type: 'INCLUDE'],
            [pattern: 'testresults', type: 'INCLUDE']
            ])
        }   
      // }
    }
}