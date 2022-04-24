# ELK-docker-compose-with-filebeat-metricbeat
We Introduce ELK Stack Using Docker for Efficient Systems.  
We derived an efficient ELK stack system based on several test results and constructed it using a docker.

# System Configuration
![image](https://user-images.githubusercontent.com/88313282/164979118-7fa03c9e-35a6-406a-a582-b9deccaa7b90.png)

# Build docker-compose file
You must set the docker-compose.yml file as follows from the above system configuration.   


If you want to add more individual instances, you can increase filebeat, elasticsearch, logstash by referring to the created **docker-compose.yml**.     


```
version: '3.3'
services:

# Elasticsearch 1 
  es01:
    image: docker.elastic.co/elasticsearch/elasticsearch:7.16.3
    deploy:
      resources:
        limits:
          cpus: '8' 
          memory: 16G
    container_name: es01
    environment:
      - node.name=es01
      - cluster.name=es-docker-clusters
      - discovery.seed_hosts=es02
      - cluster.initial_master_nodes=es01,es02
      - bootstrap.memory_lock=true
      # - "ES_JAVA_OPTS=-Xms2g -Xmx2g"
    ulimits:
      memlock:
        soft: -1
        hard: -1
    volumes:
      - data1:/usr/share/elasticsearch/data
    ports:
      - 9200:9200
    networks:
      - elastic
      
# Elasticsearch 2       
  es02:
    image: docker.elastic.co/elasticsearch/elasticsearch:7.16.3
    deploy:
      resources:
        limits:
          cpus: '8'
          memory: 16G
    container_name: es02
    environment:
      - node.name=es02
      - cluster.name=es-docker-clusters
      - discovery.seed_hosts=es01
      - cluster.initial_master_nodes=es01,es02
      - bootstrap.memory_lock=true
      # - "ES_JAVA_OPTS=-Xms2g -Xmx2g"
    ulimits:
      memlock:
        soft: -1
        hard: -1
    volumes:
      - data2:/usr/share/elasticsearch/data
    networks:
      - elastic
      
# Kibana
  kib01:
    image: docker.elastic.co/kibana/kibana:7.16.3
    deploy:
      resources:
        limits:
          cpus: '12'
          memory: 8G
    container_name: kib01
    ports:
      - 5601:5601
    environment:
      ELASTICSEARCH_URL: http://es01:9200
      ELASTICSEARCH_HOSTS: http://es01:9200
    networks:
      - elastic

# Logstash 1
  logstash:
    container_name: logstash
    image: logstash:7.16.3
    ulimits:
      memlock:
        soft: -1
        hard: -1
    volumes:
      - ./logstash.conf:/usr/share/logstash/pipeline/logstash.conf
      - ./logstash.template.json:/usr/share/logstash/templates/logstash.template.json
      - ./pipelines.yml:/usr/share/logstash/config/pipelines.yml 
    ports:
      - 5044:5044
    environment:
      ELASTICSEARCH_URL: http://es01:9200
      ELASTICSEARCH_HOSTS: http://es01:9200
    networks:
      - elastic
    logging:
      driver: "json-file"
      options:
        max-size: "10m"
        max-file: "50"
        
# Logstash 2 
  logstash2:
    container_name: logstash2
    image: logstash:7.16.3
    deploy:
      resources:
        limits:
          cpus: '12'
          memory: 8G
    ulimits:
      memlock:
        soft: -1
        hard: -1
    volumes:
      - ./logstash2.conf:/usr/share/logstash/pipeline/logstash.conf
      - ./logstash.template2.json:/usr/share/logstash/templates/logstash.template.json
      - ./pipelines2.yml:/usr/share/logstash/config/pipelines.yml 
    ports:
      - 5045:5045
    environment:
      ELASTICSEARCH_URL: http://es02:9200
      ELASTICSEARCH_HOSTS: http://es02:9200
    networks:
      - elastic
    logging:
      driver: "json-file"
      options:
        max-size: "10m"
        max-file: "50"

# Filebeat 1        
  filebeat:
    user: root
    container_name: filebeat
    image: docker.elastic.co/beats/filebeat:7.16.3
    volumes:
      - /var/run/docker.sock:/host_docker/docker.sock
      - /var/lib/docker:/host_docker/var/lib/docker
      - ./log_gen:/usr/share/filebeat/log_gen
      - ./filebeat.yml:/usr/share/filebeat/filebeat.yml
    command: ["--strict.perms=false"]
    ulimits:
      memlock:
        soft: -1
        hard: -1
    deploy:
      mode: global
    environment:
      LOGSTASH_URL: http://logstash:5044
      LOHSTASH_HOSTS: http://logstash:5044
    networks:
      - elastic
    logging:
      driver: "json-file"
      options:
        max-size: "10m"
        max-file: "50"
        
# Filebeat 2          
  filebeat2:
    user: root
    container_name: filebeat2
    image: docker.elastic.co/beats/filebeat:7.16.3
    volumes:
      - /var/run/docker.sock:/host_docker/docker.sock
      - /var/lib/docker:/host_docker/var/lib/docker
      - ./log_gen2:/usr/share/filebeat/log_gen
      - ./filebeat2.yml:/usr/share/filebeat/filebeat.yml
    command: ["--strict.perms=false"]
    ulimits:
      memlock:
        soft: -1
        hard: -1
    deploy:
      mode: global
    environment:
      LOGSTASH_URL: http://logstash2:5045
      LOHSTASH_HOSTS: http://logstash2:5045
    networks:
      - elastic
    logging:
      driver: "json-file"
      options:
        max-size: "10m"
        max-file: "50"   
        
volumes:
  data1:
    driver: local
  data2:
    driver: local
    
networks:
  elastic:
    driver: bridge  
    
```

We also ran up to 5 filebeat, 5 logstash, and 5 ElasticSearch.


# How to use

1. Docker & Docker compose install in Ubuntu
```
# Docker install
curl https://get.docker.com/ | sudo sh

# Docker install confirm
docker version

# Docker service start
sudo systemctl start docker

# Add current user to docker group
sudo usermod -aG docker {username}

# Docker-compose install 
sudo apt-get install docker-compose
```
2. Git clone
```
git clone https://github.com/hj-joo/Efficient_Distributed_Environment_ELK_System_with_docker.git
cd Efficient_Distributed_Environment_ELK_System_with_docker/
```
3. Docker-compose up
```
docker-compose up -d
```
4. Make log data
```
# 0. Install gcc compiler
sudo apt-get install gcc

# 1. Make log1 data
cd log_gen
gcc mklog1.c
cd ..

# 2. Make log2 data
cd log_gen2
gcc mklog2.c
cd ..

# 3. Create 2 log data files at the same time
cd log_gen/;./a.out;cd ..;cd log_gen2/;./a.out
cd ..
```
5. Check the log data stored in the Elasticsearch index through the Kibana in real time.
6. View running docker containers and computing resources
```
# docker container confirm
docker ps

# computing resources
docker stats
```
7. Docker-compose stop
```
docker-compose stop
```
