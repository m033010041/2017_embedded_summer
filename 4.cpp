#include <stddef.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct header_t {
    size_t size;
    unsigned is_free;
    struct header_t *next;
} *head, *tail;

static struct header_t *
get_free_block(size_t size) {
    struct header_t *curr = head;
    while (curr) {
        if (curr->is_free && curr->size >= size) return curr;
        curr = curr->next;
    }
    return NULL;
}

pthread_mutex_t global_malloc_lock;
pthread_mutex_t global_free_lock;

void *mymalloc(size_t size) {
    pthread_mutex_lock(&global_malloc_lock);
    size_t total_size;
    void *block;
    struct header_t *header;
    struct header_t *tmp;
    if (!size) return NULL;
    if (header = get_free_block(size)) {
        header->is_free = 0; 
        if (size+2*sizeof(struct header_t) < header->size){  // reduce waste
            tmp = header + size/sizeof(struct header_t); 
            tmp->size = header->size - size/sizeof(struct header_t);
            tmp->next = header->next;
            tmp->is_free = 1;
            header->next = tmp;
        }    
        return header + 1; /* FIXME */
    }

    total_size = sizeof(struct header_t) + size;
    if ((block = sbrk(total_size)) == (void *) -1)
        return NULL;

    header = block;
    header->size = size;
    header->is_free = 0;
    header->next = NULL;
    /* FIXME */
    if(head == NULL)
        head = header;
    else
        tail->next = header;
    tail = header;
    pthread_mutex_unlock(&global_malloc_lock);
    return header + 1; // + sizeof(struct header_t); 
    /* FIXME */
}


void myfree(void *ptr) {
    pthread_mutex_lock(&global_malloc_lock);
    struct header_t *target = (header_t *) ptr - 1;
    struct header_t *pre = NULL;
    struct header_t *curr = head;
    if (ptr == NULL)
        return;
    while (curr) {
        if (curr == target){
            curr->is_free = 1;
            if( curr->next && curr->next->is_free){
                curr->size += curr->next->size;
                curr->next =  curr->next->next;
            }
            if( pre && pre->is_free){
                pre->size += curr->size;
                pre->next =  curr->next;
            }
            break;
        }
        pre = curr;
        curr = pre->next;
    }
    pthread_mutex_unlock(&global_malloc_lock);
}

void *test(void *argu){
    int *a = (int *)mymalloc(100*sizeof(int));
    int *b = (int *)mymalloc(sizeof(int));
    int *c = (int *)mymalloc(sizeof(int));
    *a = 100;
    *b = 110;
    *c = 120;
    printf("%x\n",a);
    printf("%x\n",b);
    printf("%x\n",c);
    printf("%d\n",*a);
    printf("%d\n",*b);
    printf("%d\n",*c);
    myfree(a);
    myfree(b);
    myfree(c);
    int *d = (int *)mymalloc(100*sizeof(int));
    int *e = (int *)mymalloc(100*sizeof(int));
    int *f = (int *)mymalloc(100*sizeof(int));
    printf("%x\n",d);
    printf("%x\n",e);
    printf("%x\n",f);

}


int main()
{
    pthread_t th[3];
    int i;
    for(i=0;i<3;i++)
        pthread_create(&th[i],NULL,&test,NULL);
    for(i=0;i<3;i++)
        pthread_join(th[i],NULL);   //join main thread with all thread
    return 0;
}


