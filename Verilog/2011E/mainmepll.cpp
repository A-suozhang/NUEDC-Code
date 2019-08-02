#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "mseries.h"

using namespace std;
constexpr auto PI = 3.14156265;

int main() {
	ofstream fil;
	fil.open("./out.txt");
	double fclk = 10e6, dr = 101e3;
	int pts = 50000;

	double tnmax = pts / fclk;
	double sr = dr * 2;

	double crc = 200e3;
	double crm = 0;

	int dn = 0;
	int dnlast = 0;
	int dnedge = 0;
	int dnedgecnt = 0;

	int cn = 0;
	int cnlast = 0;
	int cnedge = 0;
	int cnedgecnt = 0;

	double pd = 0.0;
	double vcophase = 0.4;
	for (double tn = 0.0; tn < tnmax; tn += 1.0 / fclk) {
		int dati = ((int)(tn * sr)) % 510;
		vcophase += (crc+crm) / fclk;
		dn = sigdat[dati];
		cn = ((int)vcophase) % 2;
		fil << dn << " ";
		fil << cn << " ";

		if ((dn == 1 && dnlast == 0) /*|| (dn == 0 && dnlast == 1)*/) {
			dnedgecnt = 40;
			dnedge = 1;
		} else {
			if (dnedgecnt > 0) {
				dnedgecnt--;
				dnedge = 1;
			} else {
				dnedge = 0;
			}
		}
		dnlast = dn;
		fil << dnedge << " ";

		if ((cn == 1 && cnlast == 0) || (cn == 0 && cnlast == 1)) {
			cnedgecnt = 40;
			cnedge = 1;
		} else {
			if (cnedgecnt > 0) {
				cnedgecnt--;
				cnedge = 1;
			} else {
				cnedge = 0;
			}
		}
		cnlast = cn;
		fil << cnedge << " ";

		if (dnedge == 1) {
			if(dnedgecnt<40/2)
				pd = pd * 0.999 + cnedge * 0.001;
			else 
				pd = pd * 0.999 + (-1.0)*cnedge * 0.001;
		}
		fil << pd << endl;

		crm = pd * 100000;
	}
	fil.close();
	return 0;
}