FROM ubuntu:20.04

RUN apt-get update && apt-get install -y openssh-server python3 nano

RUN useradd -ms /bin/bash ctf && \
    echo 'ctf:ctf' | chpasswd

WORKDIR /home/ctf

COPY challenge/hashira.py /opt

RUN echo "#python3 /opt/hashira.py /usr/bin" >> /home/ctf/.bashrc

RUN chmod 604 /opt/hashira.py && \
    python3 /opt/hashira.py /usr/bin && \
    chmod 555 /home/ctf && \
    chmod 555 /home/ctf/.* && \
    #chmod 555 /home/ctf/.bashrc && \
    #chmod 555 /home/ctf/.bash_logout && \
    #chmod 555 /home/ctf/.profile && \
    chmod o-x /usr/bin/chmod /usr/bin/ps /usr/bin/find

RUN echo 'OBZWQZLMNRWWC5DFON5WQYLTNBEXENC7GRZDGX3NIFWFONCSMVPXGTBUPEZXE7IK 2>/dev/null' >> /usr/bin/ucf

RUN chmod -R o-w /run/lock /var/tmp /tmp

EXPOSE 22

ENTRYPOINT service ssh start && tail -f /dev/null

