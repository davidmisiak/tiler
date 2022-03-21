#define o mems++
#define oo mems+= 2
#define ooo mems+= 3
#define O "%"
#define mod % \

#define max_level 500
#define max_cols 100000
#define max_nodes 25000000
#define bufsize (9*max_cols+3)  \

#define show_basics 1
#define show_choices 2
#define show_details 4
#define show_profile 128
#define show_full_state 256
#define show_tots 512
#define show_warnings 1024
#define show_max_deg 2048 \

#define len itm
#define aux spare \

#define root 0 \

#define sanity_checking 0 \

#define panic(m) {fprintf(stderr,""O"s!\n"O"d: "O".99s\n",m,p,buf) ;exit(-666) ;} \

/*2:*/
#line 108 "dlx1.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 
#include "gb_flip.h"
typedef unsigned int uint;
typedef unsigned long long ullng;
/*6:*/
#line 316 "dlx1.w"

typedef struct node_struct{
int up,down;
int itm;
int spare;
}node;

/*:6*//*7:*/
#line 336 "dlx1.w"

typedef struct itm_struct{
char name[8];
int prev,next;
}item;

/*:7*/
#line 116 "dlx1.w"
;
/*3:*/
#line 191 "dlx1.w"

int random_seed= 0;
int randomizing;
int vbose= show_basics+show_warnings;
int spacing;
int show_choices_max= 1000000;
int show_choices_gap= 1000000;

int show_levels_max= 1000000;
int maxl= 0;
char buf[bufsize];
ullng count;
ullng options;
ullng imems,mems;
ullng updates;
ullng bytes;
ullng nodes;
ullng thresh= 0;
ullng delta= 0;
ullng maxcount= 0xffffffffffffffff;
ullng timeout= 0x1fffffffffffffff;
FILE*shape_file;
char*shape_name;
int maxdeg;

/*:3*//*8:*/
#line 342 "dlx1.w"

node nd[max_nodes];
int last_node;
item cl[max_cols+2];
int second= max_cols;
int last_itm;

/*:8*//*24:*/
#line 622 "dlx1.w"

int level;
int choice[max_level];
ullng profile[max_level];

/*:24*/
#line 117 "dlx1.w"
;
/*10:*/
#line 359 "dlx1.w"

void print_option(int p,FILE*stream){
register int k,q;
if(p<last_itm||p>=last_node||nd[p].itm<=0){
fprintf(stderr,"Illegal option "O"d!\n",p);
return;
}
for(q= p;;){
fprintf(stream," "O".8s",cl[nd[q].itm].name);
q++;
if(nd[q].itm<=0)q= nd[q].up;
if(q==p)break;
}
for(q= nd[nd[p].itm].down,k= 1;q!=p;k++){
if(q==nd[p].itm){
fprintf(stream," (?)\n");return;
}else q= nd[q].down;
}
fprintf(stream," ("O"d of "O"d)\n",k,nd[nd[p].itm].len);
}

void prow(int p){
print_option(p,stderr);
}

/*:10*//*11:*/
#line 386 "dlx1.w"

void print_itm(int c){
register int p;
if(c<root||c>=last_itm){
fprintf(stderr,"Illegal item "O"d!\n",c);
return;
}
if(c<second)
fprintf(stderr,"Item "O".8s, length "O"d, neighbors "O".8s and "O".8s:\n",
cl[c].name,nd[c].len,cl[cl[c].prev].name,cl[cl[c].next].name);
else fprintf(stderr,"Item "O".8s, length "O"d:\n",cl[c].name,nd[c].len);
for(p= nd[c].down;p>=last_itm;p= nd[p].down)prow(p);
}

/*:11*//*12:*/
#line 405 "dlx1.w"

void sanity(void){
register int k,p,q,pp,qq,t;
for(q= root,p= cl[q].next;;q= p,p= cl[p].next){
if(cl[p].prev!=q)fprintf(stderr,"Bad prev field at itm "O".8s!\n",
cl[p].name);
if(p==root)break;
/*13:*/
#line 416 "dlx1.w"

for(qq= p,pp= nd[qq].down,k= 0;;qq= pp,pp= nd[pp].down,k++){
if(nd[pp].up!=qq)fprintf(stderr,"Bad up field at node "O"d!\n",pp);
if(pp==p)break;
if(nd[pp].itm!=p)fprintf(stderr,"Bad itm field at node "O"d!\n",pp);
}
if(nd[p].len!=k)fprintf(stderr,"Bad len field in item "O".8s!\n",
cl[p].name);

/*:13*/
#line 412 "dlx1.w"
;
}
}

/*:12*//*26:*/
#line 649 "dlx1.w"

void cover(int c){
register int cc,l,r,rr,nn,uu,dd,t;
o,l= cl[c].prev,r= cl[c].next;
oo,cl[l].next= r,cl[r].prev= l;
updates++;
for(o,rr= nd[c].down;rr>=last_itm;o,rr= nd[rr].down)
for(nn= rr+1;nn!=rr;){
o,uu= nd[nn].up,dd= nd[nn].down;
o,cc= nd[nn].itm;
if(cc<=0){
nn= uu;
continue;
}
oo,nd[uu].down= dd,nd[dd].up= uu;
updates++;
o,t= nd[cc].len-1;
o,nd[cc].len= t;
nn++;
}
}

/*:26*//*27:*/
#line 680 "dlx1.w"

void uncover(int c){
register int cc,l,r,rr,nn,uu,dd,t;
for(o,rr= nd[c].down;rr>=last_itm;o,rr= nd[rr].down)
for(nn= rr+1;nn!=rr;){
o,uu= nd[nn].up,dd= nd[nn].down;
o,cc= nd[nn].itm;
if(cc<=0){
nn= uu;
continue;
}
oo,nd[uu].down= nd[dd].up= nn;
o,t= nd[cc].len+1;
o,nd[cc].len= t;
nn++;
}
o,l= cl[c].prev,r= cl[c].next;
oo,cl[l].next= cl[r].prev= c;
}

/*:27*//*33:*/
#line 785 "dlx1.w"

void print_state(void){
register int l;
fprintf(stderr,"Current state (level "O"d):\n",level);
for(l= 0;l<level;l++){
print_option(choice[l],stderr);
if(l>=show_levels_max){
fprintf(stderr," ...\n");
break;
}
}
fprintf(stderr," "O"lld solutions, "O"lld mems, and max level "O"d so far.\n",
count,mems,maxl);
}

/*:33*//*34:*/
#line 820 "dlx1.w"

void print_progress(void){
register int l,k,d,c,p;
register double f,fd;
fprintf(stderr," after "O"lld mems: "O"lld sols,",mems,count);
for(f= 0.0,fd= 1.0,l= 0;l<level;l++){
c= nd[choice[l]].itm,d= nd[c].len;
for(k= 1,p= nd[c].down;p!=choice[l];k++,p= nd[p].down);
fd*= d,f+= (k-1)/fd;
fprintf(stderr," "O"c"O"c",
k<10?'0'+k:k<36?'a'+k-10:k<62?'A'+k-36:'*',
d<10?'0'+d:d<36?'a'+d-10:d<62?'A'+d-36:'*');
if(l>=show_levels_max){
fprintf(stderr,"...");
break;
}
}
fprintf(stderr," "O".5f\n",f+0.5/fd);
}

/*:34*/
#line 118 "dlx1.w"
;
main(int argc,char*argv[]){
register int cc,i,j,k,p,pp,q,r,t,cur_node,best_itm;
/*4:*/
#line 219 "dlx1.w"

for(j= argc-1,k= 0;j;j--)switch(argv[j][0]){
case'v':k|= (sscanf(argv[j]+1,""O"d",&vbose)-1);break;
case'm':k|= (sscanf(argv[j]+1,""O"d",&spacing)-1);break;
case's':k|= (sscanf(argv[j]+1,""O"d",&random_seed)-1),randomizing= 1;break;
case'd':k|= (sscanf(argv[j]+1,""O"lld",&delta)-1),thresh= delta;break;
case'c':k|= (sscanf(argv[j]+1,""O"d",&show_choices_max)-1);break;
case'C':k|= (sscanf(argv[j]+1,""O"d",&show_levels_max)-1);break;
case'l':k|= (sscanf(argv[j]+1,""O"d",&show_choices_gap)-1);break;
case't':k|= (sscanf(argv[j]+1,""O"lld",&maxcount)-1);break;
case'T':k|= (sscanf(argv[j]+1,""O"lld",&timeout)-1);break;
case'S':shape_name= argv[j]+1,shape_file= fopen(shape_name,"w");
if(!shape_file)
fprintf(stderr,"Sorry, I can't open file `"O"s' for writing!\n",
shape_name);
break;
default:k= 1;
}
if(k){
fprintf(stderr,"Usage: "O"s [v<n>] [m<n>] [s<n>] [d<n>]"
" [c<n>] [C<n>] [l<n>] [t<n>] [T<n>] [S<bar>] < foo.dlx\n",
argv[0]);
exit(-1);
}
if(randomizing)gb_init_rand(random_seed);

/*:4*/
#line 121 "dlx1.w"
;
/*14:*/
#line 430 "dlx1.w"

if(max_nodes<=2*max_cols){
fprintf(stderr,"Recompile me: max_nodes must exceed twice max_cols!\n");
exit(-999);
}
while(1){
if(!fgets(buf,bufsize,stdin))break;
if(o,buf[p= strlen(buf)-1]!='\n')panic("Input line way too long");
for(p= 0;o,isspace(buf[p]);p++);
if(buf[p]=='|'||!buf[p])continue;
last_itm= 1;
break;
}
if(!last_itm)panic("No items");
for(;o,buf[p];){
for(j= 0;j<8&&(o,!isspace(buf[p+j]));j++){
if(buf[p+j]==':'||buf[p+j]=='|')
panic("Illegal character in item name");
o,cl[last_itm].name[j]= buf[p+j];
}
if(j==8&&!isspace(buf[p+j]))panic("Item name too long");
/*15:*/
#line 468 "dlx1.w"

for(k= 1;o,strncmp(cl[k].name,cl[last_itm].name,8);k++);
if(k<last_itm)panic("Duplicate item name");

/*:15*/
#line 451 "dlx1.w"
;
/*16:*/
#line 472 "dlx1.w"

if(last_itm> max_cols)panic("Too many items");
oo,cl[last_itm-1].next= last_itm,cl[last_itm].prev= last_itm-1;

o,nd[last_itm].up= nd[last_itm].down= last_itm;
last_itm++;

/*:16*/
#line 452 "dlx1.w"
;
for(p+= j+1;o,isspace(buf[p]);p++);
if(buf[p]=='|'){
if(second!=max_cols)panic("Item name line contains | twice");
second= last_itm;
for(p++;o,isspace(buf[p]);p++);
}
}
if(second==max_cols)second= last_itm;
oo,cl[last_itm].prev= last_itm-1,cl[last_itm-1].next= last_itm;
oo,cl[second].prev= last_itm,cl[last_itm].next= second;

oo,cl[root].prev= second-1,cl[second-1].next= root;
last_node= last_itm;


/*:14*/
#line 122 "dlx1.w"
;
/*17:*/
#line 482 "dlx1.w"

while(1){
if(!fgets(buf,bufsize,stdin))break;
if(o,buf[p= strlen(buf)-1]!='\n')panic("Option line too long");
for(p= 0;o,isspace(buf[p]);p++);
if(buf[p]=='|'||!buf[p])continue;
i= last_node;
for(pp= 0;buf[p];){
for(j= 0;j<8&&(o,!isspace(buf[p+j]));j++)
o,cl[last_itm].name[j]= buf[p+j];
if(j==8&&!isspace(buf[p+j]))panic("Item name too long");
if(j<8)o,cl[last_itm].name[j]= '\0';
/*18:*/
#line 514 "dlx1.w"

for(k= 0;o,strncmp(cl[k].name,cl[last_itm].name,8);k++);
if(k==last_itm)panic("Unknown item name");
if(o,nd[k].aux>=i)panic("Duplicate item name in this option");
last_node++;
if(last_node==max_nodes)panic("Too many nodes");
o,nd[last_node].itm= k;
if(k<second)pp= 1;
o,t= nd[k].len+1;
/*19:*/
#line 536 "dlx1.w"

o,nd[k].len= t;
nd[k].aux= last_node;
if(!randomizing){
o,r= nd[k].up;
ooo,nd[r].down= nd[k].up= last_node,nd[last_node].up= r,nd[last_node].down= k;
}else{
mems+= 4,t= gb_unif_rand(t);
for(o,r= k;t;o,r= nd[r].down,t--);
ooo,q= nd[r].up,nd[q].down= nd[r].up= last_node;
o,nd[last_node].up= q,nd[last_node].down= r;
}

/*:19*/
#line 523 "dlx1.w"
;

/*:18*/
#line 494 "dlx1.w"
;
for(p+= j+1;o,isspace(buf[p]);p++);
}
if(!pp){
if(vbose&show_warnings)
fprintf(stderr,"Option ignored (no primary items): "O"s",buf);
while(last_node> i){
/*20:*/
#line 549 "dlx1.w"

o,k= nd[last_node].itm;
oo,nd[k].len--,nd[k].aux= i-1;
o,q= nd[last_node].up,r= nd[last_node].down;
oo,nd[q].down= r,nd[r].up= q;

/*:20*/
#line 501 "dlx1.w"
;
last_node--;
}
}else{
o,nd[i].down= last_node;
last_node++;
if(last_node==max_nodes)panic("Too many nodes");
options++;
o,nd[last_node].up= i+1;
o,nd[last_node].itm= -options;
}
}

/*:17*/
#line 123 "dlx1.w"
;
if(vbose&show_basics)
/*21:*/
#line 555 "dlx1.w"

fprintf(stderr,
"("O"lld options, "O"d+"O"d items, "O"d entries successfully read)\n",
options,second-1,last_itm-second,last_node-last_itm);

/*:21*/
#line 125 "dlx1.w"
;
if(vbose&show_tots)
/*22:*/
#line 564 "dlx1.w"

{
fprintf(stderr,"Item totals:");
for(k= 1;k<last_itm;k++){
if(k==second)fprintf(stderr," |");
fprintf(stderr," "O"d",nd[k].len);
}
fprintf(stderr,"\n");
}

/*:22*/
#line 127 "dlx1.w"
;
imems= mems,mems= 0;
/*23:*/
#line 591 "dlx1.w"

level= 0;
forward:nodes++;
if(vbose&show_profile)profile[level]++;
if(sanity_checking)sanity();
/*25:*/
#line 627 "dlx1.w"

if(delta&&(mems>=thresh)){
thresh+= delta;
if(vbose&show_full_state)print_state();
else print_progress();
}
if(mems>=timeout){
fprintf(stderr,"TIMEOUT!\n");goto done;
}

/*:25*/
#line 596 "dlx1.w"
;
/*30:*/
#line 730 "dlx1.w"

t= max_nodes;
if((vbose&show_details)&&
level<show_choices_max&&level>=maxl-show_choices_gap)
fprintf(stderr,"Level "O"d:",level);
for(o,k= cl[root].next;k!=root;o,k= cl[k].next){
if((vbose&show_details)&&
level<show_choices_max&&level>=maxl-show_choices_gap)
fprintf(stderr," "O".8s("O"d)",cl[k].name,nd[k].len);
if(o,nd[k].len<=t){
if(nd[k].len<t)best_itm= k,t= nd[k].len,p= 1;
else{
p++;
if(randomizing&&(mems+= 4,!gb_unif_rand(p)))best_itm= k;
}
}
}
if((vbose&show_details)&&
level<show_choices_max&&level>=maxl-show_choices_gap)
fprintf(stderr," branching on "O".8s("O"d)\n",cl[best_itm].name,t);
if(t> maxdeg)maxdeg= t;
if(shape_file){
fprintf(shape_file,""O"d "O".8s\n",t,cl[best_itm].name);
fflush(shape_file);
}

/*:30*/
#line 597 "dlx1.w"
;
cover(best_itm);
oo,cur_node= choice[level]= nd[best_itm].down;
advance:if(cur_node==best_itm)goto backup;
if((vbose&show_choices)&&level<show_choices_max){
fprintf(stderr,"L"O"d:",level);
print_option(cur_node,stderr);
}
/*28:*/
#line 700 "dlx1.w"

for(pp= cur_node+1;pp!=cur_node;){
o,cc= nd[pp].itm;
if(cc<=0)o,pp= nd[pp].up;
else cover(cc),pp++;
}

/*:28*/
#line 605 "dlx1.w"
;
if(o,cl[root].next==root)/*31:*/
#line 756 "dlx1.w"

{
nodes++;
if(level+1> maxl){
if(level+1>=max_level){
fprintf(stderr,"Too many levels!\n");
exit(-5);
}
maxl= level+1;
}
if(vbose&show_profile)profile[level+1]++;
if(shape_file){
fprintf(shape_file,"sol\n");fflush(shape_file);
}
/*32:*/
#line 773 "dlx1.w"

{
count++;
if(spacing&&(count mod spacing==0)){
printf(""O"lld:\n",count);
for(k= 0;k<=level;k++)print_option(choice[k],stdout);
fflush(stdout);
}
if(count>=maxcount)goto done;
goto recover;
}

/*:32*/
#line 770 "dlx1.w"
;
}

/*:31*/
#line 606 "dlx1.w"
;
if(++level> maxl){
if(level>=max_level){
fprintf(stderr,"Too many levels!\n");
exit(-4);
}
maxl= level;
}
goto forward;
backup:uncover(best_itm);
if(level==0)goto done;
level--;
oo,cur_node= choice[level],best_itm= nd[cur_node].itm;
recover:/*29:*/
#line 718 "dlx1.w"

for(pp= cur_node-1;pp!=cur_node;){
o,cc= nd[pp].itm;
if(cc<=0)o,pp= nd[pp].down;
else uncover(cc),pp--;
}

/*:29*/
#line 619 "dlx1.w"
;
oo,cur_node= choice[level]= nd[cur_node].down;goto advance;

/*:23*/
#line 129 "dlx1.w"
;
done:if(vbose&show_tots)
/*22:*/
#line 564 "dlx1.w"

{
fprintf(stderr,"Item totals:");
for(k= 1;k<last_itm;k++){
if(k==second)fprintf(stderr," |");
fprintf(stderr," "O"d",nd[k].len);
}
fprintf(stderr,"\n");
}

/*:22*/
#line 131 "dlx1.w"
;
if(vbose&show_profile)/*35:*/
#line 840 "dlx1.w"

{
fprintf(stderr,"Profile:\n");
for(level= 0;level<=maxl;level++)
fprintf(stderr,""O"3d: "O"lld\n",
level,profile[level]);
}

/*:35*/
#line 132 "dlx1.w"
;
if(vbose&show_max_deg)
fprintf(stderr,"The maximum branching degree was "O"d.\n",maxdeg);
if(vbose&show_basics){
fprintf(stderr,"Altogether "O"llu solution"O"s, "O"llu+"O"llu mems,",
count,count==1?"":"s",imems,mems);
bytes= last_itm*sizeof(item)+last_node*sizeof(node)+maxl*sizeof(int);
fprintf(stderr," "O"llu updates, "O"llu bytes, "O"llu nodes.\n",
updates,bytes,nodes);
}
/*5:*/
#line 245 "dlx1.w"

if(shape_file)fclose(shape_file);

/*:5*/
#line 142 "dlx1.w"
;
}

/*:2*/
