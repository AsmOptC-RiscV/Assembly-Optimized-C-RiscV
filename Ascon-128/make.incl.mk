OPTS := -O3
# Ascon-128/Permutation/%.o: CFLAGS += -O1
Ascon-128/crypto_aead/opt64_C2_unrolled/%.o: OPTS := -O1
Ascon-128/crypto_aead/opt64_C_unrolled/%.o: OPTS := -O1
Ascon-128/crypto_aead/bi32/%.o: OPTS := -O1
