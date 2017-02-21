#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list_head {
	struct list_head *next;
	struct list_head *prev;
};

struct threadParameter {
	struct list_head list;
	int number;
};

struct list_head tp_head;

void list_head_init(struct list_head * head_ptr){
	head_ptr->next = head_ptr;
	head_ptr->prev = head_ptr;
}

struct threadParameter *my_entry_of(struct threadParameter *ptr, struct list_head *head){ //ptr, member
	//(type *)( (char *)ptr - offsetof(type,member) );
	ptr = (struct threadParameter *)(head);
	return ptr;
}

void golist(void) {
	struct list_head *now = &tp_head;
	int i =0;
	now = now->next;
	while(now != &tp_head){	//to-do something with ptr->*
		//ptr = my_entry_of(ptr,now);
		i++;
		printf("%d->",i);
		now = now->next;
	}
	printf("\n");
}

void my_list_add_tail(struct list_head *new, struct list_head *head){
	struct list_head *tail = head->prev;
	tail->next = new;
	new->prev = tail;
	head->prev = new;
	new->next = head;
	//golist();
}

void my_list_del(struct list_head *it){
	struct list_head *p = it->prev;
	struct list_head *n = it->next;
	p->next = n;
	n->prev = p;
	golist();
	//free(it);
}

void tp_link_free(struct threadParameter *entry){
	my_list_del(&(entry->list));
	free(entry);
	golist();
}

void tp_link_free_all(void) {
	struct list_head *now = &tp_head;
	struct list_head *next = now->next;
	struct threadParameter *ptr;
	now = next;
	next = next->next;
	while(now!=&tp_head){	//to-do something with ptr->*
		ptr = my_entry_of(ptr,now);
		my_list_del(&ptr->list);
		printf("goting to send SIGUSR2 to a thread\n" );
		//if(pthread_kill(*(ptr->thread_pointer),SIGUSR2)!=0)
			//printf("pthread_kill error");
		golist();
		printf("sent success\n" );
		free(ptr);
		now = next;
		next = now->next;
	}
}

int
main(){
	list_head_init( &tp_head );
	for(int i=0;i<6;i++){
		struct threadParameter * threadP = malloc(sizeof(struct threadParameter));
		threadP->number = i;
		//printf("%d\n", i);
		printf("%d\n", &threadP->list);
		my_list_add_tail(&threadP->list, &tp_head);
		golist();
	}

	tp_link_free_all();

	return 0;

}
