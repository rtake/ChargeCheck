# include "Rtlib/src/Rtlib.h"

int main(int argc, char *argv[]) {
  int num,natom;
  FILE *fp;
  char line[2048], header[] = "#p UHF/6-31G Pop=NPA";
  vector< vector<Atom> > mols;

  fp = fopen(argv[1],"r");

  for(num=0;num<1000000 && fgets(line,2048,fp);num++) {
    sscanf(line,"%d",&natom);
    fgets(line,2048,fp);

    vector<Atom> mol(natom);
    char gjf[256],sub[256];
    FILE *fpg;
 
    sprintf(gjf,"popl%d.gjf",num); // cout << gjf << endl;
    sprintf(sub,"g09 %s",gjf);

    for(int i=0;i<natom;i++) {
      fgets(line,2048,fp);
      mol[i].SetfromString(line);
    }

    fpg = fopen(gjf,"w");
    fprintf(fpg,"%s\n\n",header);
    fprintf(fpg,"population analysis for %s (%d)\n\n",argv[1],num);
    fprintf(fpg,"1 1\n");
    for(int i=0;i<natom;i++) { fprintf(fpg,"%s%17.12lf%17.12lf%17.12lf\n",mol[i].GetElm().c_str(),mol[i].GetCrd(0),mol[i].GetCrd(1),mol[i].GetCrd(2)); }
    fprintf(fpg,"\n");
    fclose(fpg);

    printf("%s\n",sub);
    system(sub);
  }


  return 0;
}
