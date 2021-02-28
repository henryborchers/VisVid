FROM jenkins/jenkinsfile-runner:1.0-beta-26
RUN apt update && apt install -y git
RUN apt-get update && \
    apt-get -y install apt-transport-https \
      ca-certificates \
      curl \
      gnupg2 \
      software-properties-common && \
    curl -fsSL https://download.docker.com/linux/$(. /etc/os-release; echo "$ID")/gpg > /tmp/dkey; apt-key add /tmp/dkey && \
    add-apt-repository \
      "deb [arch=amd64] https://download.docker.com/linux/$(. /etc/os-release; echo "$ID") \
      $(lsb_release -cs) \
      stable" && \
   apt-get update && \
   apt-get -y install docker-ce


COPY ci/jenkins/plugins.txt /usr/share/jenkins/ref/plugins.txt
RUN cd /app/jenkins && jar -cvf jenkins.war * &&  java -jar /app/bin/jenkins-plugin-manager.jar --war /app/jenkins/jenkins.war --plugin-file /usr/share/jenkins/ref/plugins.txt --latest-specified
COPY ci/jenkins/jenkins.yml  /usr/share/jenkins/ref/casc/jenkins.yaml
#COPY * /workspace/
ENV JAVA_OPTS="-Dorg.jenkinsci.plugins.durabletask.BourneShellScript.LAUNCH_DIAGNOSTICS=true"
#COPY Jenkinsfile /workspace/Jenkinsfile
