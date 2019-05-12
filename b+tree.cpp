#include<bits/stdc++.h>
using namespace std;
#define pb push_back
#define mp make_pair
#define f first
#define s second
#define sc(n) scanf("%d",&n)
#define scl(n) scanf("%lld",&n)
#define pr(n) printf("%d",n)
#define prl(n) printf("%lld",n)
#define nl printf("\n")
#define fr(i,n) for(i=0;i<n;i++)
#define rep(i,st,en) for(i=st;i<=en;i++)
#define repv(i,en,st) for(i=en;i>=st;i--)
#define fout cout<<fixed<<setprecision(7)
#define bi(n) __builtin_popcount(n)
#define bil(n) __builtin_popcountll(n)


typedef long long ll;
typedef pair<int,int> pii;
const int N = 100010;
ll mod = 1e9+7;
ll fmod(ll b,ll exp){
    ll res =1;
    while(exp){if(exp&1ll)res=(res*b)%mod;
        b =(b*b)%mod;exp/=2ll;
    }
    return res;
}
const int deg = 40;
/* Node is element of a B+ tree node
    val - Value of the node
    cnt - Count of nodes with same value     
*/
struct node
{
    int val,cnt; 
    node(){ val=0;cnt=0;}
    node(int _val){
        val = _val;
        cnt =1 ;
    }
};

/* Bnode is one node of a B+ tree
     nodes   - the nodes stored 
     ptrs    - pointers to other B+ tree nodes
     par     - pointer to parent
     degree  - maximum number of elements in a Btree node 
     numNodes- number of elements currently in that node
     totNodes- total number of nodes in entire subtree inclusive of current node
     isLeaf  - boolean true if it is a leaf and vice-versa
*/ 
struct Bnode
{
    vector<node> nodes;
    vector<Bnode *> ptrs;
    Bnode * par;
    int degree,totNodes;
    int numNodes;
    bool isLeaf;
    Bnode(int _degree=3)
    {
        degree = _degree;
        isLeaf = true;
        numNodes = 0;
        nodes.resize(degree+1,node());
        ptrs.resize(degree+2,NULL);
        par = NULL;
        totNodes=0;
    }
    void print(){
        for(auto j:nodes)cout<<j.val<<" ";
        cout<<"\n";
    }

};

Bnode * root;
/*
    Inserting element a at the right position in B+ tree node cur,
    with lft and rt denoting its repective pointers.
*/
void insertbyShift(node a,Bnode *cur,Bnode * lft,Bnode * rt)
{
    int num = (cur->numNodes);
    //Find the position and also shift the nodes and pointers
    while(num>0 && (cur->nodes)[num-1].val>a.val)
    {
        (cur->nodes)[num] = (cur->nodes)[num-1];
        (cur->ptrs)[num+1]=(cur->ptrs)[num];
        num--;
    }
    //Update the entries for that position
    (cur->nodes)[num] = a;
    (cur->ptrs)[num]= lft;
    (cur->ptrs)[num+1] = rt;
    (cur->numNodes)++;

}
/*
    Returns the left part of new B+tree node after splitting cur
*/
Bnode * getLeftSplit(Bnode * cur)
{
    Bnode *p = new Bnode(deg);
    (p->isLeaf) = (cur->isLeaf);             
    (p->par) = (cur->par);
    int sz = (cur->numNodes-1)/2;
    //First clone the nodes
    for(int i=0;i<sz;i++)
    {
        (p->nodes)[i] = (cur->nodes)[i];
        (p->numNodes)++;
        //if its a leaf totNodes are same as sum of individual counts
        if(p->isLeaf)(p->totNodes)+=(cur->nodes)[i].cnt;
    }
    //Then the pointers
    for(int i=0;i<=sz;i++){
        (p->ptrs)[i]= (cur->ptrs)[i];
        if((cur->ptrs)[i]){
            //summing up nodes of children like segtree
            (p->totNodes)+= ((cur->ptrs)[i]->totNodes);
            (p->ptrs)[i]->par = p;
        }
    }
    return p;
}
/*
    Returns the right part of new B+tree node after splitting cur
*/
Bnode * getRightSplit(Bnode * cur)
{
    Bnode *p = new Bnode(deg);
    (p->isLeaf) = (cur->isLeaf);             
    (p->par) = (cur->par);
    int sz = (cur->numNodes-1)/2;
    int lol =0;
    int start = sz+1;
    //IMP: if it is a leaf then the middle should also be there
    if(cur->isLeaf)start = sz;
    //First clone the nodes
    for(int i=start,j=0;i<(cur->numNodes);i++,j++)
    {
        (p->nodes)[j] = (cur->nodes)[i];
        (p->numNodes)++;
        if(p->isLeaf)(p->totNodes)+=(cur->nodes)[i].cnt;
    }
    for(int i=start,j=0;i<=(cur->numNodes);i++,j++)
    {
        (p->ptrs)[j]= (cur->ptrs)[i];
        if((cur->ptrs)[i]){
            (p->totNodes)+= ((cur->ptrs)[i]->totNodes);
            (p->ptrs)[j]->par = p;
        }
    }
    return p;
}

/*
    Returns the node to be pushed upward after splitting cur
*/
node getnodetoInsert(Bnode * cur)
{
    return (cur->nodes)[(cur->numNodes-1)/2];
}


/*
    Split function to ensure B+ tree's property 
*/
void insertbySplit(Bnode * cur)
{
    Bnode * p = (cur->par);
    if( p == NULL)
    {
        //topmost node reached hence new root
        p = new Bnode(deg);
        p->isLeaf = false;
        root = p;
        cur->par =  p;
    }
    Bnode * lft = getLeftSplit(cur);
    Bnode * rt =  getRightSplit(cur);
    node nodetoInsert  = getnodetoInsert(cur);

    insertbyShift(nodetoInsert,p,lft,rt);
    //Recurse upwards
    if((p->numNodes) > deg)insertbySplit(p);    
}


/*
    Checking if a value is present in a particular B+tree node,
    if yes then increase its count
*/
bool isPresent(Bnode *x,int value){
    for(int i=0;i<(x->numNodes);i++){
        if((x->nodes)[i].val == value)
        {
            (x->nodes)[i].cnt++;
            return true;
        }
    }
    return false;
}

/*
    Insert function for B+tree , a -> new element
*/
void insert(node &a){
    Bnode * x = root;
    //Root is NULL, then create new root
    if(x == NULL){
        root = new Bnode(deg);
        x = root;
        (root->numNodes)++;
        (root->nodes)[0]= a;
        (root->totNodes)=1;
        return;
    }
    else{
        //while not at leaf find the right node to go down
        while((x->isLeaf) == false){
            int cur= 0;
            //Increment totNodes for the node you are in 
            x->totNodes++;
            while(cur<(x->numNodes) && (x->nodes)[cur].val<=a.val)
                cur++;

            x = (x->ptrs)[cur];
        }
    }
    x->totNodes++;
    //Check if node is already present
    if(isPresent(x,a.val))return;
    insertbyShift(a,x,NULL,NULL);
    if((x->numNodes)<=deg)
        return;

    else insertbySplit(x);
}
/*
    Printing the entire tree
*/
void printTree(Bnode *x,int dep=0,int num=0)
{
    if(x==NULL)return;
    cout<<dep<<" "<<num<<" ";
    x->print();
    int lol = 0;
    for(auto j:(x->ptrs))if(j)printTree(j,dep+1,lol++);
}

/*
    Finding count of a particular value in B+ tree
*/
int findCnt(int val)
{
    Bnode *x = root;
    if(x==NULL)return 0;
    //Go down to the leaf
    while((x->isLeaf) == false){
        int cur= 0;
        while(cur<(x->numNodes) && (x->nodes)[cur].val<=val)
            cur++;

        x = (x->ptrs)[cur];
    }
    if(x==NULL){return 0;}
    for(int i=0;i<(x->numNodes);i++)
        if((x->nodes)[i].val == val)
            return (x->nodes)[i].cnt;
    return 0;

}
/*
    Find count of all elements in subtree of x
     with value >= given value
*/
int getCntforGreater(Bnode *x,int value)
{
    if(x == NULL)return 0;
    int ans = 0,i=0;
    //if leaf just iterate over all elements
    if(x->isLeaf){
        for(i=0;i<(x->numNodes);i++)
            if((x->nodes)[i].val >= value)
                ans+= (x->nodes)[i].cnt;
        return ans;
    }
    //For all nodes having values >= value, use totNodes for their childrens answers
    for(i=(x->numNodes)-1;i>=0;i--)
        if((x->nodes)[i].val >= value)
            ans+= ((x->ptrs)[i+1]->totNodes);
        else break;
    //Recurse further down
    ans += getCntforGreater((x->ptrs)[i+1],value);
    return ans;
}
/*
    Find count of all elements in subtree of x
     with value < given value
*/
int getCntforLesser(Bnode *x,int value)
{
    if(x == NULL)return 0;
    int ans = 0,i=0;
    if(x->isLeaf){
        for(i=0;i<(x->numNodes);i++)
            if((x->nodes)[i].val < value)
                ans+= (x->nodes)[i].cnt;
        return ans;
    }
    for(i=0;i<(x->numNodes);i++)
        if((x->nodes)[i].val < value)
            ans+= ((x->ptrs)[i]->totNodes);
        else break;
    ans += getCntforLesser((x->ptrs)[i],value);
    return ans;
}

/*
    Find count of all elements in subtree of x
     with value in range [l,r)
*/
int rngCnt(int l,int r)
{
    Bnode *x = root;
    if(x==NULL)return 0;
    int ans = 0;
    while((x->isLeaf) == false){
        //Find intervals between which values lie
        int st = 0,en = (x->numNodes);
        while(st<(x->numNodes) && l>=(x->nodes)[st].val)
            st++;
        while(en>0 && r<(x->nodes)[en-1].val)
            en--;
        //If same go further down
        if(st == en)x = (x->ptrs)[st];
        else
        {
            //Add up answers for intervals in between
            for(int i=st+1;i<en;i++)
                ans += (x->ptrs)[i]->totNodes;
            //Get answers for individual nodes
            ans += getCntforGreater((x->ptrs)[st],l);
            ans += getCntforLesser((x->ptrs)[en],r);
            return ans;
        }
    }
    //For leaf just iterate over all elements
    for(int i=0;i<(x->numNodes);i++){
        if((x->nodes)[i].val>=l && ((x->nodes)[i].val<r))
            ans += (x->nodes)[i].cnt;
    }
    return ans;
}



string solve(const string &p)
{
    int m;
    int found = p.find(" ");
    string cmd = p.substr(0,found);
    string args = p.substr(found+1,p.length()-found-1);
    if(cmd=="INSERT"){
        m = stoi(args);
        node a = node(m);
        insert(a);
        return "";
    }
    else if(cmd=="FIND")
    {
        m = stoi(args);
        int total = findCnt(m);
        if(total)return "YES";
        else return "NO";
    }
    else if(cmd=="COUNT"){
        m = stoi(args);
        int total = findCnt(m);
        return to_string(total);
    }
    else if(cmd == "RANGE"){
        int found = args.find(" ");
        string arg1 = args.substr(0,found);
        string arg2 = args.substr(found+1,args.length()-found-1);
        int x = stoi(arg1),y = stoi(arg2);
        if(x>y)swap(x,y);
        return to_string(rngCnt(x,y+1));
    }
    exit(1);
}


struct BufferManager{
    vector<vector<string> > inpbuf;
    vector<string> outbuf;
    int M,B;
    BufferManager(int _M,int _B){
        M = _M/8;
        B = _B/8;
        inpbuf.clear();outbuf.clear();
    }
    bool addtoBuffer(string &cmd)
    {
        int en = inpbuf.size()-1;
        if(en<0 || inpbuf[en].size()== B){
            vector<string>  new_vec;
            new_vec.pb(cmd);
            inpbuf.pb(new_vec);
        }
        else inpbuf[en].pb(cmd);
        if( en == M-2)
            return (inpbuf[en].size()==B);
        return false;
    }
    void operate(){
       
        for(int i=0;i<inpbuf.size();i++){
            for(int j=0;j<inpbuf[i].size();j++){
                string ans = solve(inpbuf[i][j]);
                if(ans.size()>0)outbuf.pb(ans);
                if(outbuf.size()== B)
                    print();
            }
            inpbuf[i].clear();
        }
        print();
        inpbuf.clear();
    }
    void print(){
        for(int i=0;i<outbuf.size();i++)
            cout<<outbuf[i]<<"\n";
        outbuf.clear();
    }
};

string s;
int main(int argc, const char *argv[])
{
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    //ios_base::sync_with_stdio(false);cin.tie(NULL);
    int t=1,n,i,j,m,q;
    if(argc!=4){
        cout<<"Enter 3 arguments- filename M B\n";
        return 0;
    }
    int M = atoi(argv[2]),B= atoi(argv[3]);
    if(!(B>0 && M >=2 && M<= (1000000/B)))
    {
        cout<<"M>=2 and M*B<=10^6 - condition not met!!";
        return 0;
    }
    ifstream infile(argv[1]);
    BufferManager buffer = BufferManager(atoi(argv[2]),atoi(argv[3]));
    while(getline(infile,s)){ 
        bool areBuffersOver = buffer.addtoBuffer(s);
        if(areBuffersOver)
            buffer.operate();

    }
    buffer.operate();

    return 0;
}
