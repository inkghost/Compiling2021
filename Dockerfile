FROM gcc:10
WORKDIR /app/
COPY ./* /app/
RUN gcc main.c -o main
WORKDIR /app/output