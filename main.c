#include "mysock.h"

int count = 0;
void *handler(void *arg)
{
	NODE *pnode;
	int ret;
	while(1)
	{
		if(head.next != NULL)
		{
			pnode = head.next;
			head.next = head.next->next;

			if(pnode->msghead.msgtype == MSGTYPE_PRODUCE)
			{
				printf("num = %d\n", pnode->msghead.num);
				count = count + pnode->msghead.num;
				printf("count = %d\n",count);
				char *pstr = "I have recv msg";
				ret = write(pnode->fd, pstr, strlen(pstr));
				if(ret < 0)
				{
					printf("write error\n");
					return NULL;
				}


			}
			else if(pnode->msghead.msgtype == MSGTYPE_USE)
			{
				if(pnode->msghead.num > count)
				{
					char *pstr = "use error";
					ret = write(pnode->fd, pstr, strlen(pstr));
					if(ret < 0)
					{
						printf("write error\n");
						return NULL;
					}
				}
				else
				{
					count = count - pnode->msghead.num;
					char *pstr = "use ok";
					ret = write(pnode->fd, pstr, strlen(pstr));
					if(ret < 0)
					{
						printf("write error\n");
						return NULL;
					}
				}
			}

		}
	}
	return NULL;
}
void *createServerHandler(void *arg)
{
	int ret;

    printf("arg = %s\n",(char *)arg);
	ret = creatServer(atoi((char *)arg));

	return NULL;
}


int main(int argc, char ** argv)
{
	int ret;
	pthread_t pth_id;
    pthread_t pth_id1;

	memset(&head, 0, sizeof(head));

	ret = pthread_create(&pth_id, NULL, handler, NULL);
	if(ret < 0)
	{
		printf("create error\n");
		return -1;
	}
    
	printf("*********\n");
	ret = pthread_create(&pth_id1, NULL,createServerHandler,argv[1]);
	if (ret <0)
	{
		printf("create error\n");
		return -1;
	}

	pthread_join(pth_id,NULL);
	pthread_join(pth_id1,NULL);
	return 0;
}
