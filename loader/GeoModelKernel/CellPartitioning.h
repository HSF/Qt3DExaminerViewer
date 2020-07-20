/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#ifndef GEOMODELKERNEL_CELLPARTITIONING_H
#define GEOMODELKERNEL_CELLPARTITIONING_H
#include <vector>

class CellPartitioning
{
  public:

    CellPartitioning(unsigned int firstDivisionNumber = 0);

    CellPartitioning(const CellPartitioning& right);

    virtual ~CellPartitioning();

    CellPartitioning& operator=(const CellPartitioning& right);

    void addValue(double xVal);

    double binLower(unsigned int i) const
    {
      return m_partition[i-getFirstDivisionNumber()];
    }

    double binUpper(unsigned int i) const
    {
      return m_partition[i-getFirstDivisionNumber()+1];
    }

    double binCenter(unsigned int i) const
    {
      return 0.5*(binUpper(i)+binLower(i));
    }

    unsigned int getNumDivisions() const
    {
      return m_partition.size()-1;
    }

    unsigned int getFirstDivisionNumber() const
    {
      return m_firstDivisionNumber;
    }

    double getStart() const
    {
      return m_partition[0];
    }

    double getEnd() const
    {
      return m_partition[m_partition.size()-1];
    }

  private:

    std::vector<double> m_partition;

    unsigned int m_firstDivisionNumber;
};

#endif
