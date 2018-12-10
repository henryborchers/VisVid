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
        stash includes: "build/release/", name: 'RELEASE_BUILD_FILES'

//         sh '''which gcov
// git submodule init
// git submodule update
// '''
//         sh '''mkdir build
// cd build
// cmake .. -DVISVID_BUILDDOCS=ON
// cmake --build .'''
      }
    }
    stage('Test') {
      stages{
        stage("Build Debug version"){

        
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
        }
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