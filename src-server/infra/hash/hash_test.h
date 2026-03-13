#ifndef UPP_HASH_TESTER
#define UPP_HASH_TESTER

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <limits.h>

typedef void (*hash_func)(
    const void *data,
    size_t len,
    uint64_t seed,
    void *out
);

typedef struct {
    const char *name;
    size_t hash_size;
    hash_func func;
} hash_test_ctx;

static uint64_t popcount8(uint8_t x){ return __builtin_popcount(x); }

static size_t popcount_buf(uint8_t *a,uint8_t *b,size_t len){
    size_t d=0;
    for(size_t i=0;i<len;i++) d+=popcount8(a[i]^b[i]);
    return d;
}

static void rand_bytes(uint8_t *b,size_t n){
    for(size_t i=0;i<n;i++) b[i]=rand()%256;
}

static double entropy(uint8_t *buf,size_t len){
    int f[256]={0};
    for(size_t i=0;i<len;i++) f[buf[i]]++;

    double H=0;
    for(int i=0;i<256;i++){
        if(!f[i]) continue;
        double p=(double)f[i]/len;
        H-=p*log2(p);
    }
    return H;
}

static void test_entropy(hash_test_ctx *ctx)
{
    printf("\n[entropy] - [ideal: >7.875]\n");

    size_t samples = 10000;

    uint8_t *data = malloc(samples * ctx->hash_size);
    uint8_t input[64];

    for(size_t i=0;i<samples;i++)
    {
        rand_bytes(input,64);

        ctx->func(input,64,i,data + i*ctx->hash_size);
    }

    double e = entropy(data, samples * ctx->hash_size);

    printf("entropy: %.6f / 8\n", e);

    free(data);
}

static void test_collisions(hash_test_ctx *ctx){

    printf("\n[collision] - [ideal: 0]\n");

    size_t n=100000;

    uint8_t *table=malloc(n*ctx->hash_size);

    uint8_t in[32];

    for(size_t i=0;i<n;i++){
        rand_bytes(in,32);
        ctx->func(in,32,i,table+i*ctx->hash_size);
    }

    size_t col=0;

    for(size_t i=0;i<n;i++)
    for(size_t j=i+1;j<n;j++)
        if(!memcmp(table+i*ctx->hash_size,table+j*ctx->hash_size,ctx->hash_size))
            col++;

    printf("collisions: %zu / %zu\n",col,n);

    free(table);
}

static void test_avalanche(hash_test_ctx *ctx){

    printf("\n[avalanche] - [ideal: ≃50%%]\n");

    uint8_t in[64];
    rand_bytes(in,64);

    uint8_t *a=malloc(ctx->hash_size);
    uint8_t *b=malloc(ctx->hash_size);

    ctx->func(in,64,0,a);

    size_t flips=0;

    for(int bit=0;bit<512;bit++){

        uint8_t tmp[64];
        memcpy(tmp,in,64);

        tmp[bit/8]^=1<<(bit%8);

        ctx->func(tmp,64,0,b);

        flips+=popcount_buf(a,b,ctx->hash_size);
    }

    size_t bits=ctx->hash_size*8;

    double avg=(double)flips/512;

    printf("avg flipped bits: %.2f / %zu (%.2f%%)\n",
        avg,bits,100.0*avg/bits);

    free(a);
    free(b);
}

static void test_bit_bias(hash_test_ctx *ctx){

    printf("\n[bit bias] - [ideal: <0.01]\n");

    size_t samples=20000;
    size_t bits=ctx->hash_size*8;

    uint64_t *count=calloc(bits,sizeof(uint64_t));

    uint8_t in[64];
    uint8_t *out=malloc(ctx->hash_size);

    for(size_t i=0;i<samples;i++){

        rand_bytes(in,64);

        ctx->func(in,64,i,out);

        for(size_t b=0;b<bits;b++)
            if(out[b/8]&(1<<(b%8)))
                count[b]++;
    }

    double worst=0;

    for(size_t b=0;b<bits;b++){
        double p=(double)count[b]/samples;
        double bias=fabs(p-0.5);
        if(bias>worst) worst=bias;
    }

    printf("max bias: %.6f\n",worst);

    free(count);
    free(out);
}

static void test_chi2(hash_test_ctx *ctx){

    printf("\n[chi2 distribution] - [ideal: ≃255 for 256 buckets]\n");

    size_t samples=50000;
    #define BUCKETS_QUANT 256
    uint64_t bucket[BUCKETS_QUANT]={0};

    uint8_t in[64];
    uint8_t *out=malloc(ctx->hash_size);

    for(size_t i=0;i<samples;i++){

        rand_bytes(in,64);
        ctx->func(in,64,i,out);

        bucket[out[0]]++;
    }

    double exp=(double)samples/256;

    double chi2=0;

    for(int i=0;i<256;i++){
        double d=bucket[i]-exp;
        chi2+=d*d/exp;
    }

    printf("chi2: %.2f for %d buckets\n",chi2, BUCKETS_QUANT);

    free(out);
}

static void test_speed(hash_test_ctx *ctx){

    printf("\n[speed single]\n");

    uint8_t in[64];
    rand_bytes(in,64);

    uint8_t *out=malloc(ctx->hash_size);

    size_t rounds=200000;

    clock_t s=clock();

    for(size_t i=0;i<rounds;i++)
        ctx->func(in,64,i,out);

    clock_t e=clock();

    double sec=(double)(e-s)/CLOCKS_PER_SEC;

    printf("hashes/sec: %.2f M\n",(rounds/sec)/1e6);

    free(out);
}

static void test_speed_bulk(hash_test_ctx *ctx){

    printf("\n[speed bulk]\n");

    size_t n=200000;

    uint8_t *buf=malloc(n*32);
    uint8_t *out=malloc(ctx->hash_size);

    rand_bytes(buf,n*32);

    clock_t s=clock();

    for(size_t i=0;i<n;i++)
        ctx->func(buf+i*32,32,i,out);

    clock_t e=clock();

    double sec=(double)(e-s)/CLOCKS_PER_SEC;

    printf("bulk hashes/sec: %.2f M\n",(n/sec)/1e6);

    free(buf);
    free(out);
}

/* -------- HASH IMAGE GENERATOR -------- */

static void hsv_to_rgb(double h,double s,double v,
                       uint8_t *r,uint8_t *g,uint8_t *b){

    double c=v*s;
    double x=c*(1-fabs(fmod(h/60.0,2)-1));
    double m=v-c;

    double rr,gg,bb;

    if(h<60){rr=c;gg=x;bb=0;}
    else if(h<120){rr=x;gg=c;bb=0;}
    else if(h<180){rr=0;gg=c;bb=x;}
    else if(h<240){rr=0;gg=x;bb=c;}
    else if(h<300){rr=x;gg=0;bb=c;}
    else{rr=c;gg=0;bb=x;}

    *r=(rr+m)*255;
    *g=(gg+m)*255;
    *b=(bb+m)*255;
}

static void generate_images(hash_test_ctx *ctx){

    printf("\n[randomness image 512x512] - [ideal: like tv noise]\n");

    system("mkdir -p /tmp/hashTest");

    const int W = 512;
    const int H = 512;

    uint8_t *img = malloc(W * H);
    uint8_t *hash = malloc(ctx->hash_size);

    uint64_t counter = 0;
    size_t pixel = 0;

    while(pixel < W*H)
    {
        ctx->func(&counter,sizeof(counter),counter,hash);

        for(size_t i=0;i<ctx->hash_size && pixel < W*H;i++)
        {
            img[pixel++] = hash[i];
        }

        counter++;
    }

    FILE *f = fopen("/tmp/hashTest/randomness.ppm","wb");

    fprintf(f,"P5\n%d %d\n255\n",W,H);

    fwrite(img,1,W*H,f);

    fclose(f);

    printf("image -> /tmp/hashTest/randomness.ppm\n");

    free(img);
    free(hash);
}

static void test_avalanche_matrix(hash_test_ctx *ctx)
{
    printf("\n[avalanche matrix] - [ideal: avg 0.5, min ≃ max ≃ 0.5]\n");

    int in_bits = 256;
    int out_bits = ctx->hash_size * 8;

    int matrix[in_bits][out_bits];
    memset(matrix,0,sizeof(matrix));

    uint8_t base[32];
    uint8_t a[ctx->hash_size];
    uint8_t b[ctx->hash_size];

    int samples = 2000;

    for(int s=0;s<samples;s++)
    {
        for(int i=0;i<32;i++)
            base[i]=rand();

        ctx->func(base,32,0,a);

        for(int ib=0;ib<in_bits;ib++)
        {
            uint8_t tmp[32];
            memcpy(tmp,base,32);

            tmp[ib/8] ^= 1<<(ib%8);

            ctx->func(tmp,32,0,b);

            for(int ob=0;ob<out_bits;ob++)
            {
                if(b[ob/8] & (1<<(ob%8)))
                    if(!(a[ob/8]&(1<<(ob%8))))
                        matrix[ib][ob]++;
                if(!(b[ob/8]&(1<<(ob%8))))
                    if(a[ob/8]&(1<<(ob%8)))
                        matrix[ib][ob]++;
            }
        }
    }

    double min=1,max=0,avg=0;
    int total=in_bits*out_bits;

    for(int i=0;i<in_bits;i++)
        for(int j=0;j<out_bits;j++)
        {
            double p=(double)matrix[i][j]/samples;

            if(p<min)min=p;
            if(p>max)max=p;

            avg+=p;
        }

    avg/=total;

    printf("flip probability avg: %.4f (ideal 0.5)\n",avg);
    printf("min: %.4f max: %.4f\n",min,max);
}

static void test_bit_independence(hash_test_ctx *ctx)
{
    printf("\n[bit independence] - [ideal: ≃0.5]\n");

    size_t samples = 20000;
    size_t bits = ctx->hash_size * 8;

    uint8_t input[64];
    uint8_t *out = malloc(ctx->hash_size);

    uint64_t correlated = 0;
    uint64_t total = 0;

    for(size_t i=0;i<samples;i++)
    {
        rand_bytes(input,64);

        ctx->func(input,64,i,out);

        for(size_t a=0;a<bits;a++)
            for(size_t b=a+1;b<bits;b++)
            {
                int A = (out[a/8]>>(a%8))&1;
                int B = (out[b/8]>>(b%8))&1;

                if(A==B) correlated++;

                total++;
            }
    }

    double ratio = (double)correlated/total;

    printf("correlation ratio: %.6f\n",ratio);

    free(out);
}

static uint64_t rotl64(uint64_t x,int r)
{
    return (x<<r)|(x>>(64-r));
}

static void test_rotational(hash_test_ctx *ctx)
{
    printf("\n[rotational symmetry] - [ideal: ≃512]\n");

    uint64_t input = ((uint64_t)rand()<<32)|rand();

    uint8_t out1[256];
    uint8_t out2[256];

    ctx->func(&input,8,0,out1);

    uint64_t rot = rotl64(input,13);

    ctx->func(&rot,8,0,out2);

    size_t diff = popcount_buf(out1,out2,ctx->hash_size);

    printf("diff bits: %zu / %zu\n",
        diff, ctx->hash_size*8);
}

static void test_differential(hash_test_ctx *ctx)
{
    printf("\n[differential test] - [ideal: ≃512]\n");

    uint8_t base[64];
    rand_bytes(base,64);

    uint8_t *a = malloc(ctx->hash_size);
    uint8_t *b = malloc(ctx->hash_size);

    ctx->func(base,64,0,a);

    size_t total=0;

    for(int i=0;i<1000;i++)
    {
        uint8_t tmp[64];
        memcpy(tmp,base,64);

        int pos = rand()%64;

        tmp[pos] ^= rand();

        ctx->func(tmp,64,i,b);

        total += popcount_buf(a,b,ctx->hash_size);
    }

    double avg = (double)total / 1000;

    printf("avg diff bits: %.2f / %zu\n",
        avg, ctx->hash_size*8);

    free(a);
    free(b);
}

static void test_seed_diffusion(hash_test_ctx *ctx)
{
    printf("\n[seed diffusion] - [ideal: ≃512]\n");

    uint8_t input[64];
    rand_bytes(input,64);

    uint8_t *a = malloc(ctx->hash_size);
    uint8_t *b = malloc(ctx->hash_size);

    ctx->func(input,64,1,a);
    ctx->func(input,64,2,b);

    size_t diff = popcount_buf(a,b,ctx->hash_size);

    printf("seed diff bits: %zu / %zu\n",
        diff, ctx->hash_size*8);

    free(a);
    free(b);
}

static void test_chi2_multibyte(hash_test_ctx *ctx)
{
    printf("\n[chi2 multi-byte] - [ideal: ≃255]\n");

    size_t samples = 50000;

    uint64_t bucket[256]={0};

    uint8_t in[64];
    uint8_t *out=malloc(ctx->hash_size);

    for(size_t i=0;i<samples;i++)
    {
        rand_bytes(in,64);

        ctx->func(in,64,i,out);

        for(size_t j=0;j<ctx->hash_size;j++)
            bucket[out[j]]++;
    }

    double expected = (double)(samples*ctx->hash_size)/256;

    double chi2=0;

    for(int i=0;i<256;i++)
    {
        double d=bucket[i]-expected;
        chi2 += (d*d)/expected;
    }

    printf("chi2 total: %.2f\n",chi2);

    free(out);
}

static void test_hamming_weight(hash_test_ctx *ctx)
{
    double ideal = ctx->hash_size * 4;
    printf("\n[hamming weight] - [ideal: ≃%.2f]\n", ideal);

    uint8_t out[ctx->hash_size];
    double total = 0;

    for(int i=0;i<10000;i++)
    {
        uint64_t in = rand();

        ctx->func(&in,sizeof(in),0,out);

        int weight=0;

        for(size_t j=0;j<ctx->hash_size;j++)
            weight += popcount8(out[j]);

        total += weight;
    }

    double avg = total / 10000;

    printf("avg weight %.2f / %.2f\n",avg,ideal);
}

static void test_sequential_correlation(hash_test_ctx *ctx)
{
    printf("\n[sequential correlation] - [ideal: ≃%.2f]\n", (double)ctx->hash_size*4);

    uint8_t a[ctx->hash_size];
    uint8_t b[ctx->hash_size];

    double total=0;

    for(uint64_t i=0;i<50000;i++)
    {
        ctx->func(&i,sizeof(i),0,a);

        uint64_t j=i+1;

        ctx->func(&j,sizeof(j),0,b);

        total += popcount_buf(a,b,ctx->hash_size);
    }

    double avg = total/50000;

    printf("avg diff %.2f / %.2f\n",avg,(double)ctx->hash_size*4);
}

static void test_long_range_avalanche(hash_test_ctx *ctx)
{
    printf("\n[long range avalanche] - [ideal: ≃%.2f]\n", (double)ctx->hash_size*4);

    uint8_t a[ctx->hash_size];
    uint8_t b[ctx->hash_size];

    uint8_t input[64]={0};
    uint8_t mask[64];

    double total=0;

    for(int i=0;i<20000;i++)
    {
        for(int j=0;j<64;j++)
            mask[j]=rand();

        ctx->func(input,64,0,a);

        uint8_t tmp[64];

        for(int j=0;j<64;j++)
            tmp[j]=input[j]^mask[j];

        ctx->func(tmp,64,0,b);

        total += popcount_buf(a,b,ctx->hash_size);
    }

    double avg = total/20000;

    printf("avg diff %.2f / %.2f\n",avg,(double)ctx->hash_size*4);
}

static void test_low_entropy(hash_test_ctx *ctx)
{
    printf("\n[low entropy] - [ideal: ≃%d]\n", (int)ctx->hash_size*4);

    uint8_t patterns[4]={0x00,0xFF,0xAA,0x55};

    uint8_t buf[64];
    uint8_t out[ctx->hash_size];

    for(int p=0;p<4;p++)
    {
        memset(buf,patterns[p],64);

        ctx->func(buf,64,0,out);

        int ones=0;

        for(size_t i=0;i<ctx->hash_size;i++)
            ones+=popcount8(out[i]);

        printf("pattern %02x weight %d\n",patterns[p],ones);
    }
}

static void test_fixed_points(hash_test_ctx *ctx)
{
    printf("\n[fixed point] - [ideal: 0]\n");

    uint8_t out[ctx->hash_size];

    int found=0;

    for(uint64_t i=0;i<100000;i++)
    {
        ctx->func(&i,sizeof(i),0,out);

        if(memcmp(out,&i,sizeof(i))==0)
            found++;
    }

    printf("fixed points %d\n",found);
}

static void test_cycle(hash_test_ctx *ctx)
{
    printf("\n[cycle test] - [ideal: 0]\n");

    uint8_t tortoise[64]={0};
    uint8_t hare[64]={0};

    uint8_t out1[ctx->hash_size];
    uint8_t out2[ctx->hash_size];

    memcpy(hare,tortoise,64);

    for(int i=0;i<1000000;i++)
    {
        ctx->func(tortoise,64,0,out1);
        memcpy(tortoise,out1,64);

        ctx->func(hare,64,0,out1);
        ctx->func(out1,64,0,out2);
        memcpy(hare,out2,64);

        if(!memcmp(tortoise,hare,64))
        {
            printf("cycle detected around iteration %d\n",i);
            return;
        }
    }

    printf("no cycle detected\n");
}

static double entropy_window(uint8_t *data,int len)
{
    int freq[256]={0};

    for(int i=0;i<len;i++)
        freq[data[i]]++;

    double h=0;

    for(int i=0;i<256;i++)
    {
        if(!freq[i]) continue;

        double p=(double)freq[i]/len;
        h -= p*log2(p);
    }

    return h;
}

static void test_sliding_entropy(hash_test_ctx *ctx)
{
    printf("\n[sliding entropy] - [ideal: >4.875 per window (cap is 5 per window)]\n");

    uint8_t out[ctx->hash_size];

    uint64_t in=rand();

    ctx->func(&in,sizeof(in),0,out);

    int window=32;

    for(size_t i=0;i+window<ctx->hash_size;i+=window)
    {
        double e = entropy_window(out+i,window);

        printf("window %zu entropy %.4f\n",i,e);
    }
}

static void test_byte_pair(hash_test_ctx *ctx)
{
    printf("\n[byte pair distribution] - [ideal: ≃%d]\n", 256*256);

    static int bucket[256][256];

    memset(bucket,0,sizeof(bucket));

    uint8_t out[ctx->hash_size];

    int samples=20000;

    for(int i=0;i<samples;i++)
    {
        uint64_t in=rand();

        ctx->func(&in,sizeof(in),0,out);

        for(size_t j=0;j+1<ctx->hash_size;j++)
            bucket[out[j]][out[j+1]]++;
    }

    double expected=(double)samples*((int)ctx->hash_size-1)/(256*256);

    double chi2=0;

    for(int i=0;i<256;i++)
        for(int j=0;j<256;j++)
        {
            double diff=bucket[i][j]-expected;
            chi2 += diff*diff/expected;
        }

    printf("chi2 pair %.2f\n",chi2);
}

static void test_differential_cluster(hash_test_ctx *ctx)
{
    int ideal_min = 1;
    int ideal_max = (int)ctx->hash_size * 8 / 6;
    printf("\n[differential cluster] - [ideal: min ≃%d max ≃%d]\n", ideal_min, ideal_max);

    uint8_t a[ctx->hash_size];
    uint8_t b[ctx->hash_size];

    uint8_t input[32]={0};

    int cluster[ctx->hash_size*8];
    memset(cluster,0,sizeof(cluster));

    for(int bit=0;bit<256;bit++)
    {
        memset(input,0,32);

        ctx->func(input,32,0,a);

        input[bit/8]^=1<<(bit%8);

        ctx->func(input,32,0,b);

        for(size_t i=0;i<ctx->hash_size;i++)
        {
            uint8_t d=a[i]^b[i];

            for(int k=0;k<8;k++)
                if(d&(1<<k))
                    cluster[i*8+k]++;
        }
    }

    int min=INT_MAX,max=0;

    for(size_t i=0;i<ctx->hash_size*8;i++)
    {
        if(cluster[i]<min)min=cluster[i];
        if(cluster[i]>max)max=cluster[i];
    }

    printf("cluster min %d max %d\n",min,max);
}

static void generate_bit_influence_heatmap(hash_test_ctx *ctx)
{
    printf("\n[bit influence heatmap] - [ideal: homogeneous monochromatic color]\n");

    int in_bits = 256;
    int out_bits = ctx->hash_size*8;

    int matrix[in_bits][out_bits];
    memset(matrix,0,sizeof(matrix));

    uint8_t base[32];
    uint8_t a[ctx->hash_size];
    uint8_t b[ctx->hash_size];

    int samples=2000;

    for(int s=0;s<samples;s++)
    {
        for(int i=0;i<32;i++)
            base[i]=rand();

        ctx->func(base,32,0,a);

        for(int ib=0;ib<in_bits;ib++)
        {
            uint8_t tmp[32];
            memcpy(tmp,base,32);

            tmp[ib/8]^=1<<(ib%8);

            ctx->func(tmp,32,0,b);

            for(int ob=0;ob<out_bits;ob++)
            {
                if((a[ob/8]^(b[ob/8]))&(1<<(ob%8)))
                    matrix[ib][ob]++;
            }
        }
    }

    int W = out_bits;
    int H = in_bits;

    FILE *f=fopen("/tmp/hashTest/bit_influence.ppm","wb");

    fprintf(f,"P5\n%d %d\n255\n",W,H);

    for(int y=0;y<H;y++)
    {
        for(int x=0;x<W;x++)
        {
            double p=(double)matrix[y][x]/samples;

            uint8_t v = (uint8_t)(p*255);

            fwrite(&v,1,1,f);
        }
    }

    fclose(f);

    printf("heatmap -> /tmp/hashTest/bit_influence.ppm\n");
}

static uint8_t rotl8(uint8_t x,int r)
{
    return (x<<r)|(x>>(8-r));
}

static void test_state_symmetry(hash_test_ctx *ctx)
{
    printf("\n[state symmetry] - [ideal: ≃%d]\n", (int)ctx->hash_size*4);

    uint8_t input[64];
    uint8_t rot[64];

    uint8_t a[ctx->hash_size];
    uint8_t b[ctx->hash_size];

    double total=0;
    int tests=0;

    int samples=20000;

    for(int s=0;s<samples;s++)
    {
        for(int i=0;i<64;i++)
            input[i]=rand();

        ctx->func(input,64,0,a);

        for(int r=1;r<8;r++)
        {
            for(int i=0;i<64;i++)
                rot[i]=rotl8(input[i],r);

            ctx->func(rot,64,0,b);

            total += popcount_buf(a,b,ctx->hash_size);
            tests++;
        }
    }

    double avg = total/tests;

    printf("rotational diff bits: %.2f / %.2f\n",
           avg,(double)ctx->hash_size*4);
}

static void test_state_symmetry_permutation(hash_test_ctx *ctx)
{
    printf("\n[state symmetry permutation] - [ideal: ~=%d]\n", (int)ctx->hash_size*4);

    uint8_t input[64];
    uint8_t perm[64];

    uint8_t a[ctx->hash_size];
    uint8_t b[ctx->hash_size];

    double total=0;

    int samples=20000;

    for(int s=0;s<samples;s++)
    {
        for(int i=0;i<64;i++)
            input[i]=rand();

        ctx->func(input,64,0,a);

        for(int i=0;i<64;i++)
            perm[i]=input[63-i];

        ctx->func(perm,64,0,b);

        total += popcount_buf(a,b,ctx->hash_size);
    }

    double avg = total/samples;

    printf("permutation diff bits %.2f / %.2f\n",
           avg,(double)ctx->hash_size*4);
}

static void test_bit_influence_variance(hash_test_ctx *ctx)
{
    printf("\n[bit influence variance] - [ideal: mean ≃0.5; lower variance is better]\n");

    int in_bits = 256;
    int out_bits = ctx->hash_size * 8;

    int matrix[in_bits][out_bits];
    memset(matrix,0,sizeof(matrix));

    uint8_t base[32];
    uint8_t a[ctx->hash_size];
    uint8_t b[ctx->hash_size];

    int samples = 2000;

    for(int s=0;s<samples;s++)
    {
        for(int i=0;i<32;i++)
            base[i]=rand();

        ctx->func(base,32,0,a);

        for(int ib=0;ib<in_bits;ib++)
        {
            uint8_t tmp[32];
            memcpy(tmp,base,32);

            tmp[ib/8] ^= 1<<(ib%8);

            ctx->func(tmp,32,0,b);

            for(int ob=0;ob<out_bits;ob++)
            {
                if((a[ob/8] ^ b[ob/8]) & (1<<(ob%8)))
                    matrix[ib][ob]++;
            }
        }
    }

    double mean=0;
    double var=0;

    int total = in_bits*out_bits;

    for(int i=0;i<in_bits;i++)
        for(int j=0;j<out_bits;j++)
        {
            double p = (double)matrix[i][j] / samples;
            mean += p;
        }

    mean /= total;

    for(int i=0;i<in_bits;i++)
        for(int j=0;j<out_bits;j++)
        {
            double p = (double)matrix[i][j] / samples;
            double d = p - mean;
            var += d*d;
        }

    var /= total;

    printf("mean influence: %.6f\n",mean);
    printf("variance: %.8f\n",var);
}

static void generate_color_image(hash_test_ctx *ctx)
{
    printf("\n[randomness color image 512x512] - [ideal: colorful tv noise]\n");

    system("mkdir -p /tmp/hashTest");

    const int W = 512;
    const int H = 512;

    uint8_t *img = malloc(W * H * 3);
    uint8_t *hash = malloc(ctx->hash_size);

    uint64_t counter = 0;
    size_t pixel = 0;

    while(pixel < W*H)
    {
        ctx->func(&counter,sizeof(counter),counter,hash);

        for(size_t i=0;i+2<ctx->hash_size && pixel < W*H;i+=3)
        {
            img[pixel*3 + 0] = hash[i];
            img[pixel*3 + 1] = hash[i+1];
            img[pixel*3 + 2] = hash[i+2];

            pixel++;
        }

        counter++;
    }

    FILE *f = fopen("/tmp/hashTest/randomness_color.ppm","wb");

    fprintf(f,"P6\n%d %d\n255\n",W,H);

    fwrite(img,1,W*H*3,f);

    fclose(f);

    printf("image -> /tmp/hashTest/randomness_color.ppm\n");

    free(img);
    free(hash);
}

static void generate_bitplane_image(hash_test_ctx *ctx)
{
    printf("\n[bitplane image] - [ideal: random pattern without lines or geometrical shapes]\n");

    system("mkdir -p /tmp/hashTest");

    int W = ctx->hash_size * 8;
    int H = 512;

    uint8_t *img = malloc(W * H);
    uint8_t *hash = malloc(ctx->hash_size);

    uint64_t counter = 0;

    for(int y=0;y<H;y++)
    {
        ctx->func(&counter,sizeof(counter),counter,hash);

        for(int x=0;x<W;x++)
        {
            int byte = x / 8;
            int bit  = x % 8;

            img[y*W + x] = (hash[byte] & (1<<bit)) ? 255 : 0;
        }

        counter++;
    }

    FILE *f = fopen("/tmp/hashTest/bitplane.ppm","wb");

    fprintf(f,"P5\n%d %d\n255\n",W,H);

    fwrite(img,1,W*H,f);

    fclose(f);

    printf("image -> /tmp/hashTest/bitplane.ppm\n");

    free(img);
    free(hash);
}

static void test_permutation(hash_test_ctx *ctx)
{
    printf("\n[permutation test] - [ideal: %d]\n", (int)ctx->hash_size*4);

    uint8_t input[64];
    uint8_t perm[64];

    uint8_t a[ctx->hash_size];
    uint8_t b[ctx->hash_size];

    double total = 0;
    int samples = 20000;

    for(int s=0;s<samples;s++)
    {
        for(int i=0;i<64;i++)
            input[i] = rand();

        ctx->func(input,64,0,a);

        /* permutação simples: reverse */
        for(int i=0;i<64;i++)
            perm[i] = input[63-i];

        ctx->func(perm,64,0,b);

        total += popcount_buf(a,b,ctx->hash_size);
    }

    double avg = total / samples;

    printf("diff bits: %.2f / %.2f\n",
           avg,(double)ctx->hash_size*4);
}

static void ht_run_all(hash_test_ctx *ctx){

    srand(time(NULL));

    printf("\n============================\n");
    printf("HASH TEST: %s\n",ctx->name);
    printf("============================\n");

    test_entropy(ctx);
    test_collisions(ctx);
    test_avalanche(ctx);
    test_bit_bias(ctx);
    test_chi2(ctx);
    test_speed(ctx);
    test_speed_bulk(ctx);
    test_avalanche_matrix(ctx);
    test_bit_independence(ctx);
    test_rotational(ctx);
    test_differential(ctx);
    test_seed_diffusion(ctx);
    test_chi2_multibyte(ctx);
    test_hamming_weight(ctx);
    test_sequential_correlation(ctx);
    test_long_range_avalanche(ctx);
    test_low_entropy(ctx);
    test_fixed_points(ctx);
    test_cycle(ctx);
    test_sliding_entropy(ctx);
    test_byte_pair(ctx);
    test_differential_cluster(ctx);
    generate_images(ctx);
    generate_bit_influence_heatmap(ctx);
    test_state_symmetry(ctx);
    test_state_symmetry_permutation(ctx);
    test_bit_influence_variance(ctx);
    generate_bitplane_image(ctx);
    generate_color_image(ctx);
    test_permutation(ctx);

}

#endif