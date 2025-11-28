# C Random Seed & rand() Replacement Examples

This repository is a collection of small C programs that demonstrate:

- Various ways to set a **random seed** for `srand()`
- Alternative **PRNG functions/APIs** that can replace `rand()`
- How to use **CSPRNG (cryptographically secure PRNG)** APIs such as `/dev/urandom`, `getrandom()`, OpenSSL `RAND_bytes()`, and libsodium `randombytes_buf()`

Each concept is implemented as a **small, self-contained C file**.

---

## Project Structure

### 1. Seed Examples (`srand()` Seed Sources)

| File | Description |
|------|------------|
| `clo.c` | Example that uses `clock()` as the seed value |
| `clocktime.c` | Uses CPU time (`clock()` family) as a seed (structure similar to `clo.c`) |
| `time_clo.c` | Uses `clock_gettime(CLOCK_MONOTONIC)` and XORs seconds/nanoseconds as the seed |
| `timeofday.c` | Uses the microsecond field (`tv_usec`) from `gettimeofday()` as the seed |
| `pid.c` | Uses `getpid()` (process ID) as the seed |
| `mem.c` | Casts the address of a stack variable to `unsigned long` and uses it as the seed |
| `urand.c` | Reads 4 bytes from `/dev/urandom` and uses that as the seed |
| `rnd_bytes.c` | Uses OpenSSL `RAND_bytes()` to generate 32 bytes of random data and print them in hex (CSPRNG example, not a seed) |
| `sc_user.c` | Reads a `seed` value from user input and calls `srand(seed)` |

All seed examples have the same basic pattern:

- `srand(…);`
- `int r = rand();`
- `printf("result : %d\n", r);`

Only the **seed source** changes from file to file.

---

### 2. `rand()` Replacement / CSPRNG Examples

| File | Description |
|------|------------|
| `randm.c` | Example using POSIX `random()` (one call without explicit seeding) |
| `drand.c` | Calls `srand48(time(NULL))`, then uses `drand48()` to generate a double in [0.0, 1.0) |
| `urandom.c` | Reads 32 bytes from `/dev/urandom` and prints them in hex (CSPRNG) |
| `get_random.c` | Uses the Linux `getrandom()` syscall to generate 32 bytes of random data (CSPRNG) |
| `randbytes.c` | Uses OpenSSL `RAND_bytes()` to generate 32 bytes of random data (CSPRNG) |
| `rndbytes_buf.c` | Uses libsodium `randombytes_buf()` to generate 16 bytes of random data (CSPRNG) |

These files **do not use `rand()` at all**.  
Each file demonstrates the basic usage of a different random API.

---

## Build Instructions

The following examples assume Linux and `gcc`.

### 1. Basic Seed Examples (No Extra Libraries)

```bash
# clock() seed
gcc clo.c -o clo

# getpid() seed
gcc pid.c -o pid

# stack address seed
gcc mem.c -o mem

# time ^ clock seed
gcc time_clo.c -o time_clo   # may require -lrt depending on glibc

# gettimeofday() seed
gcc timeofday.c -o timeofday

# /dev/urandom-based seed
gcc urand.c -o urand

# user-input seed
gcc sc_user.c -o sc_user
````

### 2. OpenSSL Examples

```bash
# OpenSSL RAND_bytes() standalone example
gcc rnd_bytes.c -o rnd_bytes -lcrypto -lssl

# rand() replacement using OpenSSL RAND_bytes()
gcc randbytes.c -o randbytes -lcrypto -lssl
```

### 3. POSIX PRNG Examples (`random()`, `drand48()`)

```bash
# random()
gcc randm.c -o randm

# drand48()
gcc drand.c -o drand
```

### 4. System CSPRNG Examples

```bash
# /dev/urandom direct usage
gcc urandom.c -o urandom_hex

# getrandom() (Linux only)
gcc get_random.c -o get_random
```

### 5. libsodium Example

```bash
# after installing libsodium
gcc rndbytes_buf.c -o rndbytes_buf -lsodium
```

---

## Example Runs

```bash
./clo
# result : 1804289383  (actual values will vary)

./pid
# result : 846930886

./urand
# result : 1681692777  (seed comes from /dev/urandom)

./urandom_hex
# result(HEX) : 9F2A7C... (32 bytes in hex)

./randm
# result : 1804289383   (result from random())

./drand
# result : 0.123456     (0.0 ~ 1.0 double)
```

Each binary is intentionally minimal:
**one execution → one random value printed.**

---

## Seed Examples – Details

### 1. `time(NULL)` / `clock()` / `time ^ clock`

* `clo.c`: `srand(clock());` → uses **CPU time** consumed by the process as a seed
* `time_clo.c`:

  ```c
  clock_gettime(CLOCK_MONOTONIC, &ts);
  srand(ts.tv_nsec ^ ts.tv_sec);
  ```

  Uses XOR of `tv_sec` (seconds) and `tv_nsec` (nanoseconds) from a monotonic clock. This gives a finer-grained seed than `time()` alone.

**Characteristics**

* Lower collision probability than using only `time()`
* Still **not suitable as a cryptographic seed** (predictable)

---

### 2. `getpid()`-based Seed (`pid.c`)

```c
srand(getpid());
int r = rand();
```

* Different process → different PID → likely different seed.

**Notes**

* PID patterns may be predictable depending on the OS
* PIDs can be reused in a short period → unsuitable for security

---

### 3. Stack Address Seed (`mem.c`)

```c
int x;
srand((unsigned long)&x);
```

* Uses the address of a stack variable as the seed, so the seed can change depending on stack layout.

**Characteristics**

* With ASLR enabled, addresses vary between executions → more diversity
* Still vulnerable if memory layout is predictable (e.g., ASLR disabled)

---

### 4. `/dev/urandom`-based Seed (`urand.c`)

```c
unsigned int seed;
int fd = open("/dev/urandom", O_RDONLY);
read(fd, &seed, sizeof(seed));
close(fd);
srand(seed);
```

* Uses **kernel entropy** from `/dev/urandom` to derive the seed.

**Pros**

* Seed itself is CSPRNG-based → much safer than time/PID/etc.
* However, `rand()` is still a weak PRNG even with a strong seed.

---

### 5. `gettimeofday()`-based Seed (`timeofday.c`)

```c
struct timeval tv;
gettimeofday(&tv, NULL);
srand(tv.tv_usec);
```

* Uses microseconds (`tv_usec`) as the seed
* More fine-grained than `time(NULL)`

---

### 6. User-Input Seed (`sc_user.c`)

```c
unsigned int seed;
printf("input seed : ");
scanf("%u", &seed);
srand(seed);
```

* User manually specifies the seed
* **Very useful for testing** when you need reproducible random sequences

---

### 7. OpenSSL `RAND_bytes()` Example (`rnd_bytes.c`)

```c
unsigned char buf[32];
if (RAND_bytes(buf, sizeof(buf)) != 1) { ... }
```

* Demonstrates **pure CSPRNG usage**, not seeding `rand()`
* Generates 32 bytes (256 bits) of random data and prints them in hex

---

## rand() Replacement Examples – Details

### 1. `random()` (`randm.c`)

```c
long x = random();
printf("result : %ld\n", x);
```

* POSIX general-purpose PRNG that is typically better than `rand()`
* Can be seeded with `srandom()`

---

### 2. `drand48()` (`drand.c`)

```c
srand48(time(NULL));
double x = drand48(); // 0.0 ~ 1.0
```

* 48-bit LCG-based PRNG
* Higher quality than `rand()`
* Directly returns a double in [0.0, 1.0)

---

### 3. `/dev/urandom` Direct Usage (`urandom.c`)

```c
unsigned char buf[32];
int fd = open("/dev/urandom", O_RDONLY);
read(fd, buf, sizeof(buf));
```

* Reads **random bytes directly**, no `rand()` at all
* True CSPRNG usage example

---

### 4. `getrandom()` (`get_random.c`)

```c
unsigned char buf[32];
ssize_t ret = getrandom(buf, sizeof(buf), 0);
```

* Linux syscall (3.17+) for CSPRNG
* Cleaner interface than opening `/dev/urandom` manually

---

### 5. OpenSSL `RAND_bytes()` (`randbytes.c`)

```c
unsigned char buf[32];
if (RAND_bytes(buf, sizeof(buf)) != 1) { ... }
```

* Uses OpenSSL’s DRBG as a CSPRNG
* Suitable for generating keys, IVs, tokens, etc.

---

### 6. libsodium `randombytes_buf()` (`rndbytes_buf.c`)

```c
if (sodium_init() < 0) { ... }
unsigned char buf[16];
randombytes_buf(buf, sizeof(buf));
```

* CSPRNG provided by the **libsodium** crypto library
* Internally uses the OS CSPRNG (Linux/Windows/macOS)

---

## Summary

* **Seed examples**:

  * Use `time`, `clock`, `getpid`, stack address, `gettimeofday`, `/dev/urandom`, user input, OpenSSL, etc.
  * Let you experiment with the **trade-offs between convenience, randomness quality, and security** for different seed sources.

* **rand() replacement examples**:

  * General-purpose PRNGs: `random()`, `drand48()`
  * CSPRNGs: `/dev/urandom`, `getrandom()`, OpenSSL `RAND_bytes()`, libsodium `randombytes_buf()`
  * Provide a practical guide to choosing the right API depending on whether you need **speed**, **statistical quality**, or **cryptographic security**.

---

## License

* These examples are intended for research and educational use.
* You are free to modify and redistribute the code.
