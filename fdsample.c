main() {
    int fd;
    char buf[10];

    fd = open("fdsample.c", 0);
    read(fd, buf, 10);
    write(1, buf, 10);
    close(fd);
}