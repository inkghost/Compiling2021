FROM gcc:10
COPY ./* /app/
WORKDIR /app/
RUN gcc main.c -o main
WORKDIR /app/output