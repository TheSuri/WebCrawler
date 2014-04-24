#ifndef AD_H
#define AD_H

class Ad {

 public:
  Ad(string _company, string _keyword, float _bid) {
    bid = _bid;
    company = _company;
    keyword = _keyword;
  }

  ~Ad() { }

   float getBid() const { return bid; }
   string getCompany() const { return company; }

 private:
  float bid;
  string keyword;
  string company;
};

#endif
