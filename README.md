# C 난수 시드 & rand() 대체 함수 예제 모음

이 저장소는 C 언어에서 **난수 시드(seed)를 설정하는 다양한 방법**과  
`rand()`를 대신할 수 있는 **여러 난수 생성 함수/API 예제 코드**를 모아둔 프로젝트입니다.

- `srand()`에 넣을 수 있는 여러 종류의 시드 소스(time, clock, PID, 메모리 주소, /dev/urandom, OpenSSL 등)
- `rand()` 대신 사용할 수 있는 일반 PRNG (`random()`, `drand48()` 등)
- 보안 난수 생성용 CSPRNG API(`/dev/urandom`, `getrandom()`, OpenSSL `RAND_bytes()`, libsodium `randombytes_buf()` 등)

을 각각 **작은 단일 C 파일 예제**로 분리하여 정리했습니다.

---

## 프로젝트 구조

### 1. Seed 예제 (`srand()` 시드 설정 방법)

| 파일 | 설명 |
|------|------|
| `clo.c` | `clock()` 값을 그대로 시드로 사용하는 예제 |
| `clocktime.c` | CPU 시간(`clock()` 계열)을 활용한 시드 예제 (구조는 `clo.c`와 유사) |
| `time_clo.c` | `clock_gettime(CLOCK_MONOTONIC)`의 초/나노초를 XOR 하여 시드로 사용 |
| `timeofday.c` | `gettimeofday()`의 마이크로초(`tv_usec`) 값을 시드로 사용 |
| `pid.c` | `getpid()`(프로세스 ID)를 시드로 사용하는 예제 |
| `mem.c` | 스택 변수의 주소를 `unsigned long`으로 캐스팅해 시드로 사용 |
| `urand.c` | `/dev/urandom`에서 4바이트를 읽어 시드로 사용 |
| `rnd_bytes.c` | OpenSSL `RAND_bytes()`로 32바이트 난수를 생성해 HEX 출력 (시드가 아니라 CSPRNG 자체 예제) |
| `sc_user.c` | 사용자로부터 `seed`를 직접 입력받아 `srand(seed)` 설정 |

각 예제는 공통적으로:

- `srand(…);`
- `int r = rand();`
- `printf("result : %d\n", r);`

형태로 **시드 소스만 다르게** 사용하는 구조입니다. 

---

### 2. `rand()` 대체 함수 / CSPRNG 예제

| 파일 | 설명 |
|------|------|
| `randm.c` | POSIX `random()` 함수 사용 예제 (시드 없이 기본 상태로 한 번 호출) :contentReference[oaicite:9]{index=9} |
| `drand.c` | `srand48(time(NULL))` 후 `drand48()`으로 0.0 ~ 1.0 실수 난수 생성 :contentReference[oaicite:10]{index=10} |
| `urandom.c` | `/dev/urandom`에서 32바이트를 읽어 HEX로 출력 (CSPRNG) :contentReference[oaicite:11]{index=11} |
| `get_random.c` | Linux `getrandom()` syscall로 32바이트 난수 생성 (CSPRNG) :contentReference[oaicite:12]{index=12} |
| `randbytes.c` | OpenSSL `RAND_bytes()`로 32바이트 난수 생성 (CSPRNG) :contentReference[oaicite:13]{index=13} |
| `rndbytes_buf.c` | libsodium `randombytes_buf()`로 16바이트 난수 생성 (CSPRNG) :contentReference[oaicite:14]{index=14} |

이들 파일은 `rand()`를 전혀 쓰지 않고, **각각 다른 난수 API의 기본 사용법**을 보여줍니다.

---

## 빌드 방법

아래 예시는 Linux/gcc 기준입니다.

### 1. 기본 seed 예제들 (추가 라이브러리 없음)

```bash
# clock() 시드
gcc clo.c -o clo

# getpid() 시드
gcc pid.c -o pid

# 메모리 주소 시드
gcc mem.c -o mem

# time ^ clock 시드
gcc time_clo.c -o time_clo   # glibc 버전에 따라 -lrt 필요할 수도 있음

# gettimeofday() 시드
gcc timeofday.c -o timeofday

# /dev/urandom 기반 시드
gcc urand.c -o urand

# 사용자 입력 seed
gcc sc_user.c -o sc_user
````

### 2. OpenSSL 예제

```bash
# OpenSSL RAND_bytes() 단독 사용 예제
gcc rnd_bytes.c -o rnd_bytes -lcrypto -lssl

# rand() 대체: OpenSSL RAND_bytes()
gcc randbytes.c -o randbytes -lcrypto -lssl
```

### 3. POSIX 난수 예제 (`random()`, `drand48()` 등)

```bash
# random()
gcc randm.c -o randm

# drand48()
gcc drand.c -o drand
```

### 4. 시스템 CSPRNG 예제

```bash
# /dev/urandom 직접 사용
gcc urandom.c -o urandom_hex

# getrandom() (Linux 전용)
gcc get_random.c -o get_random
```

### 5. libsodium 예제

```bash
# libsodium 설치 후
gcc rndbytes_buf.c -o rndbytes_buf -lsodium
```

---

## 실행 예시

```bash
./clo
# result : 1804289383  (실행마다 달라질 수 있음)

./pid
# result : 846930886

./urand
# result : 1681692777  (시드를 /dev/urandom에서 읽어옴)

./urandom_hex
# result(HEX) : 9F2A7C... (32바이트 HEX)

./randm
# result : 1804289383   (random() 결과)

./drand
# result : 0.123456     (0.0 ~ 1.0 double)
```

각 바이너리는 **한 번 실행해서 한 번의 난수**를 출력하는 가장 단순한 형태입니다.

---

## Seed 예제 상세 설명

### 1. `time(NULL)` / `clock()` / `time ^ clock`

* `clo.c` : `srand(clock());` → 프로그램이 사용한 **CPU 시간**을 시드로 사용 
* `time_clo.c` : `clock_gettime(CLOCK_MONOTONIC, &ts); srand(ts.tv_nsec ^ ts.tv_sec);`

  * 초(`tv_sec`)와 나노초(`tv_nsec`)를 XOR → 초 단위 `time()`보다 훨씬 세밀한 시드 

**특징**

* time 기반보다 충돌 확률이 줄어듦
* 여전히 **보안용 시드로는 부족** (예측 가능성 존재)

---

### 2. `getpid()` 기반 시드 (`pid.c`)

```c
srand(getpid());
int r = rand();
```

* 서로 다른 프로세스 → 서로 다른 PID → 서로 다른 시드 기대 

**주의**

* OS에 따라 PID 패턴 예측 가능
* 짧은 시간 안에는 PID 재사용 가능성도 있음 → 보안에는 부적합

---

### 3. 메모리 주소 기반 시드 (`mem.c`)

```c
int x;
srand((unsigned long)&x);
```

* 스택 변수의 주소를 시드로 사용하여 **실행마다 다른 주소값**을 활용 

**특징**

* ASLR이 켜져 있으면 매 실행마다 주소가 달라져 다양성 증가
* 하지만 메모리 레이아웃이 예측 가능한 환경에선 취약

---

### 4. `/dev/urandom` 기반 시드 (`urand.c`)

```c
unsigned int seed;
int fd = open("/dev/urandom", O_RDONLY);
read(fd, &seed, sizeof(seed));
close(fd);
srand(seed);
```

* 커널 엔트로피에서 가져온 **보안 난수**로 `rand()` 시드 설정 

**장점**

* 시드 자체는 CSPRNG 기반 → 일반적인 시드보다 훨씬 안전
* 단, 그 이후에 사용하는 `rand()` 자체는 여전히 약한 PRNG

---

### 5. `gettimeofday()` 기반 시드 (`timeofday.c`)

```c
struct timeval tv;
gettimeofday(&tv, NULL);
srand(tv.tv_usec);
```

* 마이크로초 단위(`tv_usec`)를 활용해 `time(NULL)`보다 더 미세한 시간값을 시드로 사용 

---

### 6. 사용자 입력 기반 시드 (`sc_user.c`)

```c
unsigned int seed;
printf("input seed : ");
scanf("%u", &seed);
srand(seed);
```

* 사람이 직접 seed를 지정
* **테스트/디버깅에서 동일 난수열을 재현**할 때 매우 유용 

---

### 7. OpenSSL `RAND_bytes()` 예제 (`rnd_bytes.c`)

```c
unsigned char buf[32];
if (RAND_bytes(buf, sizeof(buf)) != 1) { ... }
```

* 시드가 아니라, **순수 CSPRNG 사용법 예제**
* 32바이트(256bit) 난수를 HEX로 출력 

---

## rand() 대체 함수 예제 상세

### 1. `random()` (`randm.c`)

```c
long x = random();
printf("result : %ld\n", x);
```

* POSIX 환경에서 `rand()`보다 더 나은 일반 PRNG 
* `srandom()`으로 시드 설정 가능 (예제에서는 기본 상태 사용)

---

### 2. `drand48()` (`drand.c`)

```c
srand48(time(NULL));
double x = drand48(); // 0.0 ~ 1.0
```

* 48bit LCG 기반 PRNG, `rand()`보다 품질 우수
* 실수 난수를 직접 생성 

---

### 3. `/dev/urandom` 직접 사용 (`urandom.c`)

```c
unsigned char buf[32];
int fd = open("/dev/urandom", O_RDONLY);
read(fd, buf, sizeof(buf));
```

* 시드가 아니라 **난수 바이트 자체**를 가져오는 CSPRNG 예제 

---

### 4. `getrandom()` (`get_random.c`)

```c
unsigned char buf[32];
ssize_t ret = getrandom(buf, sizeof(buf), 0);
```

* Linux 커널 3.17+에서 제공하는 시스템 콜 기반 CSPRNG
* `/dev/urandom`보다 인터페이스가 깔끔 

---

### 5. OpenSSL `RAND_bytes()` (`randbytes.c`)

```c
unsigned char buf[32];
if (RAND_bytes(buf, sizeof(buf)) != 1) { ... }
```

* OpenSSL DRBG를 사용하는 대표적인 CSPRNG 예제 
* 키 / IV / 토큰 생성에 그대로 사용 가능

---

### 6. libsodium `randombytes_buf()` (`rndbytes_buf.c`)

```c
if (sodium_init() < 0) { ... }
unsigned char buf[16];
randombytes_buf(buf, sizeof(buf));
```

* 현대적인 암호 라이브러리 **libsodium**에서 제공하는 CSPRNG 
* 내부적으로 OS CSPRNG를 적절히 사용 (Linux/Windows/macOS)

---

## 특징 요약

* **Seed 예제**:

  * `time`, `clock`, `getpid`, `메모리 주소`, `gettimeofday`, `/dev/urandom`, 사용자 입력, OpenSSL 등
  * 각각의 시드 소스의 **장단점과 보안성 차이**를 실험해볼 수 있음.

* **rand() 대체 예제**:

  * `random()`, `drand48()` 같은 일반 PRNG
  * `/dev/urandom`, `getrandom()`, `RAND_bytes()`, `randombytes_buf()` 같은 **CSPRNG**
  * 보안 목적/실험 목적에 따라 적절한 난수 API를 선택하는 기준을 제공.

---

## 라이선스

* 연구 및 학습 목적의 예제 코드입니다.
* 자유롭게 수정 및 재배포 가능합니다.

```
```
