FROM gcc:10.2
WORKDIR /app/
COPY compiler.cpp syntax.h token.h ./
RUN g++ compiler.cpp -o compiler
RUN chmod +x compiler