pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
    }
    
  }
  stages {
    stage('Build') {
      steps {
        sh '''git submodule init
git submodule update
'''
        sh '''mkdir build
cd build
cmake ..
cmake --build .'''
      }
    }
    stage('Test') {
      steps {
        sh '''cd build
ctest'''
      }
    }
    stage('Package') {
      steps {
        sh '''cd build
cpack -G ZIP'''
        archiveArtifacts(artifacts: 'build/*.zip', fingerprint: true, onlyIfSuccessful: true)
      }
    }
    stage('Documentation') {
      steps {
        sh '''cd build
cmake --build . --target documentation
'''
        zip(zipFile: 'documentation', archive: true, dir: 'build/html')
      }
    }
  }
  post {
    always {
      echo 'cleaning up'
      deleteDir()
      
    }
    
  }
}