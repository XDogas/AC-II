unsigned char toBcd(unsigned char value) {
    return ((value / 10) << 4) + (value % 10);
}
