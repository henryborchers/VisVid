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
cmake .. -DVISVID_BUILDDOCS=ON
cmake --build .'''
      }
    }
    stage('Test') {
      steps {
        sh '''cd build
ctest'''
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
      echo 'cleaning up'
      deleteDir()
      
    }
    
  }
}