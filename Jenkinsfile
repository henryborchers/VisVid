pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
    }
    
  }
  stages {
    stage('get info') {
      steps {
        echo 'Nope'
        sh '''cmake --version
ls'''
      }
    }
    stage('build') {
      steps {
        sh '''git submodule init
git submodule update
mkdir build
cd build
cmake ..
cmake --build .'''
      }
    }
  }
}