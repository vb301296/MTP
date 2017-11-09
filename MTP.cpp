#include <bits/stdc++.h>
using namespace std;
#define DIMENSIONS 3
//#define GAMMA (double)0.000001
#define WEIGHT (double)10 
int power(int a,int b){
    if(b==0)
        return 1;
    else if(b==1)
        return a;
    else return power(a,b-1)*a;              
}

vector<int> starting_one(vector<int> a) {
    vector<int> ::iterator it = a.begin();
    while(*it != 1)
        it++;
    vector<int> temp;
    temp.insert(temp.end(),it,a.end());
    temp.insert(temp.end(),a.begin(),it);
    return temp;
}

double find_brightness_TSP(vector<int> &v,vector<vector<double> > &distances) {
    double res =0.0;
    int n = v.size();
    for(int i=0;i < n-1;i++)
        res+=distances[v[i]][v[i+1]];
    res+=distances[v[n-1]][v[0]];
    return 1.0/res;
}

vector<int> getRandomPerm(int n) {
    vector<int> ret;
    for(int i=0;i<n;i++) {
        ret.push_back(i);
    }
    random_shuffle(ret.begin(), ret.end());
    return ret;
}

vector<int> getGreedyPerm(vector<vector<double > > dist) {
    vector<int> ret;
    int temp = rand()%dist.size();
    ret.push_back(temp);
    bool visited[dist.size()]={0};
    visited[temp] = 1;
    while(ret.size()<dist.size()) {
        int mindist=INT_MAX,minindex;
        for(int i=0;i<dist.size();i++) {
            if(i!=temp&&visited[i]==0&&dist[temp][i]<=mindist) {
                mindist = dist[temp][i];
                minindex = i;
            }
        }
        visited[minindex] = 1;
        ret.push_back(minindex);
        temp = minindex;
    }
    return ret;
}

vector<vector<int> > getfireflies_TSP(int FIREFLY,int n,vector<vector<double> > v) {
    vector<vector<int> > fireflies;
    for(int i=0;i<FIREFLY;i++) {
        vector<int > firefly = getGreedyPerm(v);
        fireflies.push_back(starting_one( firefly));
    }
    return fireflies;
}

int hammingDistance(vector<int> a,vector<int> b){
    int ret = 0;
    for(int i=0;i<a.size();i++) {
        if(a[i]!=b[i])
            ret++;        
    }
    return ret;
}

int swapDistance(vector<int> a,vector<int> b){
    bool visited[a.size()] = {0};
    int rev_a[a.size()];
    for(int i=0;i<a.size();i++)
        rev_a[a[i]] = i;
    int ret = 0;
    for(int i=0;i<a.size();i++) {
        if(visited[i]==0) {
            visited[i] = true;
            int temp = rev_a[b[i]];
            while(visited[temp] == 0) {
                visited[temp] = true;
                temp = rev_a[b[temp]];
                ret++;
            }
        }
    }
    return ret;
}

double find_distance(vector<double> a,vector<double> b) {
    double dist = 0.0;
    for(int i=0;i<DIMENSIONS;i++) {
        dist += pow(a[i]-b[i],2);
    }
    return dist;
}

void print_firefly(vector<int> f) {
    for(int i=0;i<f.size();i++) {
        cout<<f[i]<<" ";
    }
    cout<<"\n";
}

vector<int> move(vector<int > firefly_from,vector<int > firefly_to ) {
    vector<int> ret = firefly_from;
    vector<int> changes;
    int hamming = 0;
    for(int i=0;i<firefly_to.size();i++){
        if(firefly_to[i]!=firefly_from[i] or (double)rand()/RAND_MAX <= 0.05) { 
            changes.push_back(i);
            hamming++;
            ret[i] = -1;
        }
    }
    random_shuffle(changes.begin(),changes.end());
    int movement = rand()%(hamming-1)+2;
    for(int i=0;i<movement;i++) {
        int index = changes[i];
        ret[index] = firefly_to[index];
    }
    int j=movement;

    for(int i=0;i<firefly_to.size();i++) {
        if(ret[i]==-1) {
            ret[i]=firefly_to[changes[j++]];
        }
    }
    return starting_one(ret);
}

bool comp(pair<vector<int>,double> a,pair<vector<int>,double> b) {

    return a.second > b.second;
}

void firefly_algo_TSP(vector<vector<int> > &population,vector<vector<double> > &distances, int FIREFLY, int MAX_GENERATION, double GAMMA, int m) {
    //int m = 5;
    double optimal = -1*DBL_MAX;
    vector<int> ans;
    //cout<<ALPHA<<" "<<BETA<<" "<<FIREFLY<<" "<<MAX_GENERATION<<" ";
    for(int i=0;i<FIREFLY;i++) {
        optimal = max(optimal,find_brightness_TSP(population[i],distances));
    }
    cout<<"Printing initial optimal "<<1/optimal<<"\n";
    for(int gen=0;gen<MAX_GENERATION;gen++) {
        cout<<"Generation No. "<<gen<<"\n";
        vector<pair<vector<int>,double> > newPopulation,pop; 
        for(int i=0;i<FIREFLY;i++) {
            int best = -1;
            double best_coeff=0;
            for(int j=0;j<FIREFLY;j++) {
                if(i!=j) {
                    if(find_brightness_TSP(population[i],distances)<find_brightness_TSP(population[j],distances)) {
                        double movement_coeff = find_brightness_TSP(population[j],distances) * exp(-1.0*GAMMA*hammingDistance(population[i],population[j]));
                        if(movement_coeff>best_coeff) {
                            best_coeff = movement_coeff;
                            best = j;
                        }
                    }
                }
            }
            cout<<"The best is "<<best<<"\n";
            if(best!=-1) {
                for(int k = 0; k < m ; k++ ) {
                    vector<int> newFirefly = population[i];
                    newFirefly = move(population[i],population[best]);
                    //cout<<find_brightness_TSP(newFirefly,distances)<<"\n";
                    newPopulation.push_back({newFirefly,find_brightness_TSP(newFirefly,distances)});
                }
            }
            else {
                newPopulation.push_back({population[i],find_brightness_TSP(population[i], distances)});
                for(int k = 0; k < m ; k++ ) {
                    vector<int > newFirefly = getRandomPerm(population[i].size());
                    newPopulation.push_back({newFirefly,find_brightness_TSP(newFirefly,distances)});
                }
            }
        }
        sort(newPopulation.begin(),newPopulation.end(),comp);
        pop.push_back(newPopulation[0]);
        for(int i=1;i<newPopulation.size();i++) {
            if(fabs(find_brightness_TSP(newPopulation[i].first,distances)-find_brightness_TSP(newPopulation[i-1].first,distances))>1e-10)
                pop.push_back(newPopulation[i]);
        }
        if(pop.size()<population.size()) {
            while(pop.size()<population.size()){
                vector<int > temp = getRandomPerm(distances.size());
                pop.push_back({temp,find_brightness_TSP(temp,distances)});
            }
        } 
        for(int i=0;i<population.size();i++) {
            population[i] = pop[i].first;
        }

        //cout<<"yahan\n";
        for(int i=0;i<FIREFLY;i++) {
            if(optimal<find_brightness_TSP(population[i],distances)){

                optimal = max(optimal,find_brightness_TSP(population[i],distances));
                ans=population[i];
            }
        }
    }
    
    cout<<GAMMA<<" "<<1/optimal<<"\n";
    for (int i = 0; i < ans.size(); ++i)
    {
        //cout<<ans[i]<<" ";

    }
}

int main() {
    srand (time(NULL));
    cout<<fixed<<setprecision(15);
    int n;
    cin>>n;
    vector<pair<double,double> > coordinates(n);
    for(int i=0;i<n;i++) {
        int x;
        cin>>x>>coordinates[i].first>>coordinates[i].second;
    }
    //cout<<"The distance matrix is\n";
    vector<vector<double> > v(n,vector<double>(n));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            v[i][j] = sqrt(pow(coordinates[i].first- coordinates[j].first,2)+pow(coordinates[i].second- coordinates[j].second,2));//rand()*1.0/RAND_MAX * 1000 + 0.0001;
            //cout<<v[i][j]<<" ";
        }
        //cout<<"\n";
    }
    //vector<int> a = {4,3,-1,1,2,5,8};
    //print_firefly(starting_one(a));

    //for(int m=5;m<50;m+=5) {
    // for(int i=0;i<10;i++) {
         vector<vector<int> > fireflies = getfireflies_TSP(100,n,v);
         firefly_algo_TSP(fireflies,v,100,1000,0.32,15);
    // }
}
