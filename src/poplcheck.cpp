# include "Rtlib/src/Rtlib.h"

int main(int argc, char *argv[]) {
  int natom=100000;
  char line[256], popl[256], frg[256];
  FILE *fp, *fp_popl, *fp_frg;
  vector<double> charge(natom);
  vector<string> atom(natom);
  vector< vector<double> > crd(natom,vector<double>(3,0));

  sprintf(popl,"%s.popl",argv[1]);
  sprintf(frg,"%s.frg",argv[1]);

  fp = fopen(argv[1],"r");
  while( fgets(line,256,fp) ) {
    if( strstr(line,"NAtoms=") ) { sscanf(line+8,"%d",&natom); }

    if( strstr(line,"Input orientation") ) {
      for(int i=0;i<5;i++) { fgets(line,256,fp); }
      for(int i=0;i<natom;i++) {
        sscanf(line+35,"%lf%lf%lf",&crd[i][0],&crd[i][1],&crd[i][2]);
        fgets(line,256,fp);
      }
    }
    
    if( strstr(line,"Summary of Natural Population Analysis") ) {
      for(int i=0;i<6;i++) { fgets(line,256,fp); }
      for(int i=0;i<natom;i++) {
        char a[5];
        sscanf(line+6,"%s",&a);
        atom[i] = a;
        sscanf(line+15,"%lf",&charge[i]);
        fgets(line,256,fp);
      }
      break;
    }
  }
  fclose(fp);

  vector< pair<int,double> > m_over_z(natom,pair<int,double>(0,0));
  vector<Atom> mol(natom);
  for(int i=0;i<natom;i++) {
    mol[i].SetElm(atom[i]);
    for(int j=0;j<3;j++) { mol[i].SetCrd(j,crd[i][j]); }
  }


  vector<int> root(natom);
  vector< vector<int> > dmat(natom,vector<int>(natom,-1));
  for(int i=0;i<natom;i++) {
    for(int j=i+1;j<natom;j++) {
      if(BondJudge(mol[i], mol[j]) == 1) {
        dmat[i][j] = 1;
        dmat[j][i] = 1;
      }
    }
  } // make distance matrix

  for(int i=0;i<natom;i++) { root[i] = i; }

  for(int i=0;i<natom;i++) {
    for(int j=i+1;j<natom;j++) {
      if(dmat[i][j] != 1) { continue; } // skip no bond

      for(int k=0;k<natom;k++) {
        if(k==j) { continue; } // same atom
        if(root[k] == root[j]) { root[k] = root[i]; }
      }
      root[ root[j] ] = root[i];
      root[j] = root[i];
    }
  }

  for(int i=0;i<natom;i++) {
    m_over_z[root[i]].first += mol[i].mass();
    m_over_z[root[i]].second += charge[i];
  }

  sort(m_over_z.begin(),m_over_z.end());

  fp_popl = fopen(popl,"w");
  fp_frg = fopen(frg,"w");
  for(int i=natom-1;i>=0;i--) { if(m_over_z[i].first > 0) { fprintf(fp_frg,"%d,",m_over_z[i].first); } }
  for(int i=natom-1;i>=0;i--) { if(m_over_z[i].first > 0) { fprintf(fp_popl,"%lf,",m_over_z[i].second); } }
  fclose(fp_popl);
  fclose(fp_frg);

  return 0;
}
