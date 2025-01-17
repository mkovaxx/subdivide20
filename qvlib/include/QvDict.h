#ifndef _QV_DICT_
#define _QV_DICT_

#include <QvBasic.h>
#include <QvString.h>
#include <QvPList.h>

class QvDictEntry {
  private:
    unsigned long		key;
    void *		value;
    QvDictEntry *	next;
    QvDictEntry(unsigned long k, void *v)	{ key = k; value = v; };

friend class QvDict;
};

class QvDict {
  public:
    QvDict( int entries = 251 );
    ~QvDict();
    void	clear();
    QvBool	enter(unsigned long key, void *value);
    QvBool	find(unsigned long key, void *&value) const;
    QvBool	remove(unsigned long key);

  private:
    int			tableSize;
    QvDictEntry *	*buckets;
    QvDictEntry *&	findEntry(unsigned long key) const;
};

#endif /* _QV_DICT_ */
