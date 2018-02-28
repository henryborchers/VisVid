pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
    }
    
  }
  stages {
    stage('build') {
      steps {
        sh '''mkdir build
git submodule init
git submodule update
cd build
cmake ..
cmake --build .'''
      }
    }
  }
}