pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
    }
    
  }
  stages {
    stage('Build') {
      steps {
        sh '''which gcov
git submodule init
git submodule update
'''
        sh '''mkdir build
cd build
cmake .. -DVISVID_BUILDDOCS=ON
cmake --build .'''
      }
    }
    stage('Test') {
      steps {
        sh 'ctest -S build.cmake --verbose'
        junit 'build/tests/test.xml'
      }
    }
    stage('Documentation') {
      steps {
        sh '''cd build
cmake --build . --target documentation
'''
        zip(zipFile: 'visvid_documentation.zip', archive: true, dir: 'build/html')
      }
    }
    stage('Package') {
      steps {
        sh '''cd build
cpack -G ZIP'''
        archiveArtifacts(artifacts: 'build/*.zip', fingerprint: true, onlyIfSuccessful: true)
      }
    }
  }
  post {
    always {
      step([$class: 'XUnitBuilder',
                                              thresholds: [
                                                            [$class: 'SkippedThreshold', failureThreshold: '0'],
                                                            [$class: 'FailedThreshold', failureThreshold: '0']],
                                                        tools: [[$class: 'CTestType', pattern: 'build/Testing/**/*.xml']]])
        echo 'cleaning up'
        deleteDir()
        
      }
      
    }
  }