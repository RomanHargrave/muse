//=========================================================
//  MusE
//  Linux Music Editor
//    $Id: alayout.cpp,v 1.8 2004/02/28 14:58:24 wschweer Exp $
//  (C) Copyright 2002 Werner Schweer (ws@seh.de)
//=========================================================

#include "alayout.h"
#include "arranger.h"

#include <QScrollBar>

//---------------------------------------------------------
//   wadd
//---------------------------------------------------------

void TLLayout::wadd(int idx, QWidget* w)
      {
      li[idx] = new QWidgetItem(w);
      if (idx == 0)
            stack = (WidgetStack*)w;
      if (idx == 1)
            sb = (QScrollBar*)w;
      addItem(li[idx]);
      }

#if 0
//---------------------------------------------------------
//   TLLayoutIterator
//---------------------------------------------------------

class TLLayoutIterator // : public QGLayoutIterator ddskrjo
      {
      int idx;
      QList<QLayoutItem*> list;

   public:
      TLLayoutIterator(QList<QLayoutItem*> l) : idx(0), list(l) {}
      QLayoutItem *current()     { return idx < int(list->count()) ? list->at(idx) : 0; }
      QLayoutItem *next()        { idx++; return current(); }
      QLayoutItem *takeCurrent() { return list->take( idx ); }
      };

//---------------------------------------------------------
//   iterator
//---------------------------------------------------------

QLayoutIterator TLLayout::iterator()
      {
      return QLayoutIterator(0); //new TLLayoutIterator(&ilist)); ddskrjo
      }

void TLLayout::addItem(QLayoutItem *item)
      {
      ilist.append(item);
      }

TLLayout::~TLLayout()
      {
      deleteAllItems();
      }

#endif

//---------------------------------------------------------
//   setGeometry
//    perform geometry management for tracklist:
//
//         0         1         2
//   +-----------+--------+---------+
//   | Trackinfo | scroll | header 2|
//   |           |   bar  +---------+ y1
//   |     ^     |        |   ^     |
//   |           |        | <list>  |
//   |     0     |   1    |    3    |
//   +-----------+--------+---------+ y2
//   |             hline     4      |
//   +----------+-------------------+ y3
//   | button 5 |                   |
//   +----------+-------------------+
//---------------------------------------------------------

void TLLayout::setGeometry(const QRect &rect)
      {
      //if(_inSetGeometry)  // p4.0.11 Tim
      //  return;
      //_inSetGeometry = true;
      
      int w = rect.width();
      int h = rect.height();

      QSize s0;
      if (stack->visibleWidget()) {
            s0 = stack->visibleWidget()->minimumSizeHint();
            if (!s0.isValid())   // widget has no geometry management
                  s0 = stack->visibleWidget()->size();
            }
      else
            s0 = stack->minimumSizeHint();

      QSize s1 = li[1]->sizeHint();
      QSize s2 = li[2]->sizeHint();
      QSize s3 = li[3]->sizeHint();
      QSize s4 = li[4]->sizeHint();
      QSize s5 = li[5]->sizeHint();

      int y1 = 30;  // fixed header height
      int ah = h - s5.height() - s4.height() - y1;   // list height
      int aw = w - s1.width() - s0.width();          // list width

      int y2 = ah + s2.height();
      int y3 = y2 + s4.height();
      int x1 = s0.width();
      int x2 = x1 + s1.width();

      li[0]->setGeometry(QRect(0,  0,  s0.width(), y2));  

      QWidget* widget = stack->visibleWidget();
      int range = s0.height() - y2;
      if (range < 0)
            range = 0;
      // Note this appears to cause a single recursive call to this function - jumps to beginning,
      //  because now the scroll bar wants to be put in the layout.
      sb->setVisible(range != 0);
      if (range)
            sb->setMaximum(range);

      if (widget) {
            //QSize r(s0.width(), y2);
            QSize r(s0.width(), y2 < s0.height() ? s0.height() : y2);   // p4.0.11 Tim
            widget->setGeometry(0, 0, r.width(), r.height()); 
            }

      li[1]->setGeometry(QRect(x1, 0,  s1.width(), y2));
      li[2]->setGeometry(QRect(x2, 0,  aw,         s2.height()));
      li[3]->setGeometry(QRect(x2, y1, aw,        ah));
      li[4]->setGeometry(QRect(0,  y2,  w,        s4.height()));
      li[5]->setGeometry(QRect(3,  y3,  s5.width(), s5.height()));
      
      //_inSetGeometry = false;
      }

//---------------------------------------------------------
//   sizeHint
//---------------------------------------------------------

QSize TLLayout::sizeHint() const
      {
      return QSize(150, 100);
      // p4.0.11 Tim. 100 was allowing vertically shrunk trackinfo widgets. Nope, no help.
      //return minimumSize();
      }

//---------------------------------------------------------
//   minimumSize
//---------------------------------------------------------

QSize TLLayout::minimumSize() const
      {
      int w = stack->minimumSizeHint().width();
      w += li[1]->sizeHint().width();
      
      return QSize(w, 50);
      // p4.0.11 Tim. 50 was allowing vertically shrunk trackinfo widgets. Nope, no help.
      //return QSize(w, stack->minimumSizeHint().height());  
      }

//---------------------------------------------------------
//   maximumSize
//---------------------------------------------------------

QSize TLLayout::maximumSize() const
      {
      return QSize(440, 100000);
      }

//---------------------------------------------------------
//   takeAt
//---------------------------------------------------------

QLayoutItem* TLLayout::takeAt(int i)
      {
      if (i >= 0 && i < ilist.size())
            return ilist.takeAt(i);
      else
            return 0;
      }

//---------------------------------------------------------
//   clear
//---------------------------------------------------------

void TLLayout::clear()
      {
      QLayoutItem* child;
      while ((child = takeAt(0)) != 0) {
            delete child->widget();
            delete child;
            }
      }