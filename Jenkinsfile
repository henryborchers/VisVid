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
cpack'''
        archiveArtifacts(artifacts: 'build/Visvid-*.*', fingerprint: true, onlyIfSuccessful: true)
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