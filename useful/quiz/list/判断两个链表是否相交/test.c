//http://blog.csdn.net/jiqiren007/article/details/6572685
struct list
{
  int data;
  struct list *next;
};

struct list *find_node(struct list *head1; struct list *head2)
{
  if(head1 == NULL || head2 == NULL)
    return NULL;//任意一个为空，不想交

  struct list *pos1, pos2;
  pos1 = head1;
  pos2 = head2;
  int len1,len2,diff;
  len1=len2=0;

  while(pos1->next != NULL) {
    pos1 = pos1->next;
    len1++;
  }

  while(pos2->next != NULL) {
    pos2 = pos2->next;
    len2++;
  }

  if(p1 != p2)
    return NULL;//尾节点不同，则不相交

  //到了这里说明已经相交，计算长度差值，然后链表头重置，把更长的链表差值步进掉，然后遍历下去看看哪个节点开始相等。
  diff = abs(len1 - len2);
  pos1 = head1;
  pos2 = head2;
  if(len1 > len2)
    for(int i=0; i<diff; i++)
      pos1 = pos1->next;
  else //len1<len2 or len1==len2
    for(int i=0; i<diff; i++)
      pos2 = pos2->next;
  while(pos1 != pos2) {
    pos1=pos1->next;
    pos2=pos2->next;
  }
  return pos1;
}
