FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

   
RUN apt-get update
RUN apt-get install -y curl  apache2 php libapache2-mod-php socat wget cron iproute2 sudo && \
    rm -rf /var/lib/apt/lists/* && \
    ln -sf /usr/bin/nc.traditional /usr/bin/nc

RUN wget https://github.com/osquery/osquery/releases/download/5.10.2/osquery_5.10.2-1.linux_amd64.deb && \
    dpkg -i osquery_5.10.2-1.linux_amd64.deb || apt-get -f install -y && \
    rm osquery_5.10.2-1.linux_amd64.deb

RUN useradd -m -s /bin/bash alice && \
    useradd -m -s /bin/bash analyst && \
    useradd -m -s /bin/bash sysadmin && \
    echo "alice:MWCRDmX8&X6h1OF2ZP07FT@wsl" | chpasswd && \
    echo "analyst:*3g^!sT9vEeRkjR75J&S*DXkY0" | chpasswd && \
    echo "sysadmin:qc2hpM2D&3gHC%2YBgRzX283iZ" | chpasswd && \
    usermod -aG sudo sysadmin

# 1. Vulnerable web app
RUN rm -rf /var/lib/apt/lists/*

RUN echo "ServerName localhost" >> /etc/apache2/apache2.conf && \
    echo "DirectoryIndex index.php" >> /etc/apache2/apache2.conf && \
    sed -i 's/AllowOverride None/AllowOverride All/g' /etc/apache2/apache2.conf

RUN mkdir -p /var/www/html/upl04ds && chmod -R 777 /var/www/html/upl04ds

COPY app/index.html /var/www/html/index.html

RUN echo "Listen 127.0.0.1:80" > /etc/apache2/ports.conf

#2. The malware
COPY app/_4fO0THoLd.php /var/www/html/upl04ds/_4fO0THoLd.php

#3. Cronjob
RUN echo "* * * * * sysadmin /home/sysadmin/_CR0N&_" > /etc/cron.d/nice-job && \
    chmod 644 /etc/cron.d/nice-job


#4. SSH Keys
RUN mkdir -p /home/sysadmin/.ssh && \
    echo "ssh-rsa JFNIX0tlM3BfVV9Jbgo= sysadmin@corp" > /home/sysadmin/.ssh/authorized_keys && \
    chown -R sysadmin:sysadmin /home/sysadmin/.ssh

#5. arp poisoning -> entrypoint

#6. sudoers
RUN mkdir -p /etc/sudoers.d && \
    echo 'sysadmin ALL=(ALL) NOPASSWD: /Nd_Sud0_4ur_amB1TiOn}' > /etc/sudoers.d/sysadmin && \
    chmod 440 /etc/sudoers.d/sysadmin


COPY osquery.conf /etc/osquery/osquery.conf
COPY entrypoint.sh /entrypoint.sh
COPY run_osquery.sh /run_osquery.sh
RUN chmod +x /entrypoint.sh /run_osquery.sh

EXPOSE 5000
ENTRYPOINT ["/entrypoint.sh"]