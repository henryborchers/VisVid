pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
    }
    
  }
  options {
    timeout(30)
    // checkoutToSubdirectory 'scm'
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
            cmakeBuild(
              buildDir: 'build/release', 
              buildType: 'Release', 
              cleanBuild: true, 
              cmakeArgs: '-DVISVID_BUILDDOCS:BOOL=ON', 
              installation: 'InSearchPath', 
              // sourceDir: 'scm',  
              steps: [[withCmake: true]]
            )
          }
          post{
            success{
              stash includes: "build/release/", name: 'RELEASE_BUILD_FILES'      
            }
          }
        }
        stage("Create Debug Build"){       
          steps {
            cmakeBuild(
              buildDir: 'build/debug', 
              buildType: 'Debug', 
              cleanBuild: true, 
              installation: 'InSearchPath', 
              cmakeArgs: "-DCTEST_DROP_LOCATION=${WORKSPACE}/reports/ctest -DCMAKE_CXX_FLAGS_DEBUG=\"-fprofile-arcs -ftest-coverage\"",
              // sourceDir: 'scm', 
              steps: [[args: '--target test-visvid', withCmake: true]]
            )
          }
          post{
            success{
              stash includes: "build/debug/", name: 'DEBUG_BUILD_FILES'      
            }
          }
        }
      }
    }
    stage('Test') {
      stages{
        
        stage("Run CTest"){
          steps{
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
              sh "gcovr -r ${WORKSPACE}/src --xml -o reports/coverage/coverage.xml build/debug"
              sh "gcovr -r ${WORKSPACE}/src--html --html-details -o reports/coverage/coverage.html build/debug"
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
                    pattern: 'testresults/*.xml',
                    skipNoTestFiles: true,
                    stopProcessingIfError: true
                    )
                  ]
        }
      }
    }
    stage('Documentation') {
      // steps {
      //   stage('Build') {
      steps {
        unstash "RELEASE_BUILD_FILES"
        cmakeBuild(
          buildDir: 'build/release', 
          buildType: 'Release', 
          cleanBuild: true, 
          cmakeArgs: '', 
          installation: 'InSearchPath', 
          // sourceDir: 'scm',  
          steps: [[args: '--target documentation', withCmake: true]]
        )
//         sh '''cd build
// cmake --build . --target documentation
// '''
//         zip(zipFile: 'visvid_documentation.zip', archive: true, dir: 'build/html')
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
          zip(zipFile: 'build/visvid_documentation.zip', archive: true, dir: 'build/release/html')
          stash includes: "build/release/html/**", name: 'DOCS_ARCHIVE'
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
      // }
    }
}