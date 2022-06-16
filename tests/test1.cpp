#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <utility>
using namespace std;

void maxProf (int a[], int n, int k) {
  int aSorted[n];
  for (int i = 0; i < n; i++) {
    aSorted[i] = a[i];
  }
  sort(aSorted, aSorted + n);

  int maxProf;
  maxProf = 0;
  for (int i = n - 1; i >= n - k; i--) {
    maxProf += aSorted[i];
  }

  cout << maxProf << "\n";
}

bool sortbysec(const pair<int,int> &a, const pair<int,int> &b) {
  return (a.second < b.second);
}

void numProb(int a[], int n, int k) {
  vector<pair<int,int> > aSorted, maxProbInd;
  for (int i = 0; i < n; i++){
    aSorted.push_back(make_pair(a[i], i + 1));
  }

  sort(aSorted.begin(), aSorted.end());
  reverse(aSorted.begin(), aSorted.end());

  for (int i = 0; i < k; i++) {
    maxProbInd.push_back(aSorted[i]);
  }

  sort(maxProbInd.begin(), maxProbInd.end(), sortbysec);

  int aux;
  aux = 0;
  for (int i = 0; i < k - 1; i++) {
    cout << maxProbInd[i].second - aux << " ";
    aux = maxProbInd[i].second;
  }
  cout << n - aux << "\n";

}

int main(int argc, char* argv[]) {
  int n, k;

  cin >> n >> k;
  int a[n];
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }

  maxProf(a, n, k);
  numProb(a, n, k);


  return 0;
}
