// Mathias Lambert V. (c) - Lab3 SEP 2018-2

unsigned char patron1(unsigned char val) {
    if (val == 0b00000001){
        val = (1 << 4);
    }
    else {
        val = (val >> 1);
    }
    return val;
}

unsigned char patron2(unsigned char val, int aux){
    if (aux == 0) {
        val = val >> 1;
    }
    else {
        val = val << 1;
    }
    return val;
}

unsigned char patrones(unsigned char val, int aux, int mode){
    if (mode == 0) {
        val = patron1(val);
    }
    else if(mode == 1){
        val = patron2(val, aux);
    }
    else{
        val = ~val;
    }
    return val;
}
