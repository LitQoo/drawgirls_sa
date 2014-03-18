/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2008-2010 Ricardo Quesada
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "CCMenuLambda.h"

using namespace std;



static std::vector<unsigned int> ccarray_to_std_vector(CCArray* pArray)
{
	std::vector<unsigned int> ret;
	CCObject* pObj;
	CCARRAY_FOREACH(pArray, pObj)
	{
		CCInteger* pInteger = (CCInteger*)pObj;
		ret.push_back((unsigned int)pInteger->getValue());
	}
	return ret;
}

enum
{
	kDefaultPadding =  5,
};

//
//CCMenuLambda
//

CCMenuLambda* CCMenuLambda::create()
{
	return CCMenuLambda::create(NULL, NULL);
}

CCMenuLambda * CCMenuLambda::create(CCMenuItemLambda* item, ...)
{
	va_list args;
	va_start(args,item);
	
	CCMenuLambda *pRet = CCMenuLambda::createWithItems(item, args);
	
	va_end(args);
	
	return pRet;
}

CCMenuLambda* CCMenuLambda::createWithArray(CCArray* pArrayOfItems)
{
	CCMenuLambda *pRet = new CCMenuLambda();
	if (pRet && pRet->initWithArray(pArrayOfItems))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	
	return pRet;
}

CCMenuLambda* CCMenuLambda::createWithItems(CCMenuItemLambda* item, va_list args)
{
	CCArray* pArray = NULL;
	if( item )
	{
		pArray = CCArray::create(item, NULL);
		CCMenuItemLambda *i = va_arg(args, CCMenuItemLambda*);
		while(i)
		{
			pArray->addObject(i);
			i = va_arg(args, CCMenuItemLambda*);
		}
	}
	
	return CCMenuLambda::createWithArray(pArray);
}

CCMenuLambda* CCMenuLambda::createWithItem(CCMenuItemLambda* item)
{
	return CCMenuLambda::create(item, NULL);
}

bool CCMenuLambda::init()
{
	return initWithArray(NULL);
}

bool CCMenuLambda::initWithArray(CCArray* pArrayOfItems)
{
	if (CCLayer::init())
	{
		setTouchPriority(kCCMenuHandlerPriority);
		setTouchMode(kCCTouchesOneByOne);
		setTouchEnabled(true);
		
		m_bEnabled = true;
		// menu in the center of the screen
		CCSize s = CCDirector::sharedDirector()->getWinSize();
		
		this->ignoreAnchorPointForPosition(true);
		setAnchorPoint(ccp(0.5f, 0.5f));
		this->setContentSize(s);
		
		setPosition(ccp(s.width/2, s.height/2));
		
		if (pArrayOfItems != NULL)
		{
			int z=0;
			CCObject* pObj = NULL;
			CCARRAY_FOREACH(pArrayOfItems, pObj)
			{
				CCMenuItemLambda* item = (CCMenuItemLambda*)pObj;
				this->addChild(item, z);
				z++;
			}
		}
    
		//    [self alignItemsVertically];
		m_pSelectedItem = NULL;
		m_eState = kCCMenuStateWaiting;
		
		// enable cascade color and opacity on menus
		setCascadeColorEnabled(true);
		setCascadeOpacityEnabled(true);
		
		return true;
	}
	return false;
}

/*
 * override add:
 */
void CCMenuLambda::addChild(CCNode * child)
{
	CCLayer::addChild(child);
}

void CCMenuLambda::addChild(CCNode * child, int zOrder)
{
	CCLayer::addChild(child, zOrder);
}

void CCMenuLambda::addChild(CCNode * child, int zOrder, int tag)
{
	CCAssert( dynamic_cast<CCMenuItemLambda*>(child) != NULL, "Menu only supports MenuItem objects as children");
	CCLayer::addChild(child, zOrder, tag);
}

void CCMenuLambda::onExit()
{
	if (m_eState == kCCMenuStateTrackingTouch)
	{
		if (m_pSelectedItem)
		{
			m_pSelectedItem->unselected();
			m_pSelectedItem = NULL;
		}
		
		m_eState = kCCMenuStateWaiting;
	}
	
	CCLayer::onExit();
}

void CCMenuLambda::removeChild(CCNode* child, bool cleanup)
{
	CCMenuItemLambda *pMenuItem = dynamic_cast<CCMenuItemLambda*>(child);
	CCAssert(pMenuItem != NULL, "Menu only supports MenuItem objects as children");
	
	if (m_pSelectedItem == pMenuItem)
	{
		m_pSelectedItem = NULL;
	}
	
	CCNode::removeChild(child, cleanup);
}

//Menu - Events

void CCMenuLambda::setHandlerPriority(int newPriority)
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->setPriority(newPriority, this);
}

void CCMenuLambda::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, this->getTouchPriority(), true); // 원래 true
}

bool CCMenuLambda::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CC_UNUSED_PARAM(event);
	
	m_touchesStart = touch->getStartLocation();
	if (m_eState != kCCMenuStateWaiting || ! m_bVisible || !m_bEnabled)
	{
		return false;
	}
	
	for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
	{
		if (c->isVisible() == false)
		{
			return false;
		}
	}
	
	m_pSelectedItem = this->itemForTouch(touch);
	if (m_pSelectedItem)
	{
		m_eState = kCCMenuStateTrackingTouch;
		m_pSelectedItem->selected();
		
		if(m_propaOnBegan)
		{
			m_pSelectedItem->activate();
		}
		return true;
	}
	return false;
}

void CCMenuLambda::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
	CC_UNUSED_PARAM(touch);
	CC_UNUSED_PARAM(event);
	
	CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchEnded] -- invalid state");
	m_touchesEnd = touch->getLocation();
	
	CCPoint difference = ccpSub(m_touchesEnd, m_touchesStart);
	
	if (difference.x > 8.0f || difference.y > 8.0f )
	{
		CCMenuLambda::ccTouchCancelled(touch, event);
	}
	else if (difference.x < -8.0f || difference.y < -8.0f)
	{
		CCMenuLambda::ccTouchCancelled(touch, event);
	}
	else
	{
		if (m_pSelectedItem)
		{
			m_pSelectedItem->unselected();
			if(m_propaOnBegan == false)
				m_pSelectedItem->activate();
		}
		m_eState = kCCMenuStateWaiting;
	}

	//////////// 여긴 원본소스.
//	if (m_pSelectedItem)
//	{
//		m_pSelectedItem->unselected();
//		m_pSelectedItem->activate();
//	}
//	m_eState = kCCMenuStateWaiting;
}

void CCMenuLambda::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{
	CC_UNUSED_PARAM(touch);
	CC_UNUSED_PARAM(event);
	CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchCancelled] -- invalid state");
	if (m_pSelectedItem)
	{
		m_pSelectedItem->unselected();
	}
	m_eState = kCCMenuStateWaiting;
}

void CCMenuLambda::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CC_UNUSED_PARAM(event);
	CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchMoved] -- invalid state");
	CCMenuItemLambda *currentItem = this->itemForTouch(touch);
	if (currentItem != m_pSelectedItem)
	{
		if (m_pSelectedItem)
		{
			m_pSelectedItem->unselected();
		}
		m_pSelectedItem = currentItem;
		if (m_pSelectedItem)
		{
			m_pSelectedItem->selected();
		}
	}
}

//Menu - Alignment
void CCMenuLambda::alignItemsVertically()
{
	this->alignItemsVerticallyWithPadding(kDefaultPadding);
}

void CCMenuLambda::alignItemsVerticallyWithPadding(float padding)
{
	float height = -padding;
	if (m_pChildren && m_pChildren->count() > 0)
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH(m_pChildren, pObject)
		{
			CCNode* pChild = dynamic_cast<CCNode*>(pObject);
			if (pChild)
			{
				height += pChild->getContentSize().height * pChild->getScaleY() + padding;
			}
		}
	}
	
	float y = height / 2.0f;
	if (m_pChildren && m_pChildren->count() > 0)
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH(m_pChildren, pObject)
		{
			CCNode* pChild = dynamic_cast<CCNode*>(pObject);
			if (pChild)
			{
				pChild->setPosition(ccp(0, y - pChild->getContentSize().height * pChild->getScaleY() / 2.0f));
				y -= pChild->getContentSize().height * pChild->getScaleY() + padding;
			}
		}
	}
}

void CCMenuLambda::alignItemsHorizontally(void)
{
	this->alignItemsHorizontallyWithPadding(kDefaultPadding);
}

void CCMenuLambda::alignItemsHorizontallyWithPadding(float padding)
{
	
	float width = -padding;
	if (m_pChildren && m_pChildren->count() > 0)
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH(m_pChildren, pObject)
		{
			CCNode* pChild = dynamic_cast<CCNode*>(pObject);
			if (pChild)
			{
				width += pChild->getContentSize().width * pChild->getScaleX() + padding;
			}
		}
	}
	
	float x = -width / 2.0f;
	if (m_pChildren && m_pChildren->count() > 0)
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH(m_pChildren, pObject)
		{
			CCNode* pChild = dynamic_cast<CCNode*>(pObject);
			if (pChild)
			{
				pChild->setPosition(ccp(x + pChild->getContentSize().width * pChild->getScaleX() / 2.0f, 0));
				x += pChild->getContentSize().width * pChild->getScaleX() + padding;
			}
		}
	}
}

void CCMenuLambda::alignItemsInColumns(unsigned int columns, ...)
{
	va_list args;
	va_start(args, columns);
	
	this->alignItemsInColumns(columns, args);
	
	va_end(args);
}

void CCMenuLambda::alignItemsInColumns(unsigned int columns, va_list args)
{
	CCArray* rows = CCArray::create();
	while (columns)
	{
		rows->addObject(CCInteger::create(columns));
		columns = va_arg(args, unsigned int);
	}
	alignItemsInColumnsWithArray(rows);
}

void CCMenuLambda::alignItemsInColumnsWithArray(CCArray* rowsArray)
{
	vector<unsigned int> rows = ccarray_to_std_vector(rowsArray);
	
	int height = -5;
	unsigned int row = 0;
	unsigned int rowHeight = 0;
	unsigned int columnsOccupied = 0;
	unsigned int rowColumns;
	
	if (m_pChildren && m_pChildren->count() > 0)
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH(m_pChildren, pObject)
		{
			CCNode* pChild = dynamic_cast<CCNode*>(pObject);
			if (pChild)
			{
				CCAssert(row < rows.size(), "");
				
				rowColumns = rows[row];
				// can not have zero columns on a row
				CCAssert(rowColumns, "");
				
				float tmp = pChild->getContentSize().height;
				rowHeight = (unsigned int)((rowHeight >= tmp || isnan(tmp)) ? rowHeight : tmp);
				
				++columnsOccupied;
				if (columnsOccupied >= rowColumns)
				{
					height += rowHeight + 5;
					
					columnsOccupied = 0;
					rowHeight = 0;
					++row;
				}
			}
		}
	}
	
	// check if too many rows/columns for available menu items
	CCAssert(! columnsOccupied, "");
	
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	
	row = 0;
	rowHeight = 0;
	rowColumns = 0;
	float w = 0.0;
	float x = 0.0;
	float y = (float)(height / 2);
	
	if (m_pChildren && m_pChildren->count() > 0)
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH(m_pChildren, pObject)
		{
			CCNode* pChild = dynamic_cast<CCNode*>(pObject);
			if (pChild)
			{
				if (rowColumns == 0)
				{
					rowColumns = rows[row];
					w = winSize.width / (1 + rowColumns);
					x = w;
				}
				
				float tmp = pChild->getContentSize().height;
				rowHeight = (unsigned int)((rowHeight >= tmp || isnan(tmp)) ? rowHeight : tmp);
				
				pChild->setPosition(ccp(x - winSize.width / 2,
																y - pChild->getContentSize().height / 2));
				
				x += w;
				++columnsOccupied;
				
				if (columnsOccupied >= rowColumns)
				{
					y -= rowHeight + 5;
					
					columnsOccupied = 0;
					rowColumns = 0;
					rowHeight = 0;
					++row;
				}
			}
		}
	}
}

void CCMenuLambda::alignItemsInRows(unsigned int rows, ...)
{
	va_list args;
	va_start(args, rows);
	
	this->alignItemsInRows(rows, args);
	
	va_end(args);
}

void CCMenuLambda::alignItemsInRows(unsigned int rows, va_list args)
{
	CCArray* pArray = CCArray::create();
	while (rows)
	{
		pArray->addObject(CCInteger::create(rows));
		rows = va_arg(args, unsigned int);
	}
	alignItemsInRowsWithArray(pArray);
}

void CCMenuLambda::alignItemsInRowsWithArray(CCArray* columnArray)
{
	vector<unsigned int> columns = ccarray_to_std_vector(columnArray);
	
	vector<unsigned int> columnWidths;
	vector<unsigned int> columnHeights;
	
	int width = -10;
	int columnHeight = -5;
	unsigned int column = 0;
	unsigned int columnWidth = 0;
	unsigned int rowsOccupied = 0;
	unsigned int columnRows;
	
	if (m_pChildren && m_pChildren->count() > 0)
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH(m_pChildren, pObject)
		{
			CCNode* pChild = dynamic_cast<CCNode*>(pObject);
			if (pChild)
			{
				// check if too many menu items for the amount of rows/columns
				CCAssert(column < columns.size(), "");
				
				columnRows = columns[column];
				// can't have zero rows on a column
				CCAssert(columnRows, "");
				
				// columnWidth = fmaxf(columnWidth, [item contentSize].width);
				float tmp = pChild->getContentSize().width;
				columnWidth = (unsigned int)((columnWidth >= tmp || isnan(tmp)) ? columnWidth : tmp);
				
				columnHeight += (int)(pChild->getContentSize().height + 5);
				++rowsOccupied;
				
				if (rowsOccupied >= columnRows)
				{
					columnWidths.push_back(columnWidth);
					columnHeights.push_back(columnHeight);
					width += columnWidth + 10;
					
					rowsOccupied = 0;
					columnWidth = 0;
					columnHeight = -5;
					++column;
				}
			}
		}
	}
	
	// check if too many rows/columns for available menu items.
	CCAssert(! rowsOccupied, "");
	
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	
	column = 0;
	columnWidth = 0;
	columnRows = 0;
	float x = (float)(-width / 2);
	float y = 0.0;
	
	if (m_pChildren && m_pChildren->count() > 0)
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH(m_pChildren, pObject)
		{
			CCNode* pChild = dynamic_cast<CCNode*>(pObject);
			if (pChild)
			{
				if (columnRows == 0)
				{
					columnRows = columns[column];
					y = (float) columnHeights[column];
				}
				
				// columnWidth = fmaxf(columnWidth, [item contentSize].width);
				float tmp = pChild->getContentSize().width;
				columnWidth = (unsigned int)((columnWidth >= tmp || isnan(tmp)) ? columnWidth : tmp);
				
				pChild->setPosition(ccp(x + columnWidths[column] / 2,
																y - winSize.height / 2));
				
				y -= pChild->getContentSize().height + 10;
				++rowsOccupied;
				
				if (rowsOccupied >= columnRows)
				{
					x += columnWidth + 5;
					rowsOccupied = 0;
					columnRows = 0;
					columnWidth = 0;
					++column;
				}
			}
		}
	}
}

CCMenuItemLambda* CCMenuLambda::itemForTouch(CCTouch *touch)
{
	CCPoint touchLocation = touch->getLocation();
	
	if (m_pChildren && m_pChildren->count() > 0)
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH(m_pChildren, pObject)
		{
			CCMenuItemLambda* pChild = dynamic_cast<CCMenuItemLambda*>(pObject);
			if (pChild && pChild->isVisible() && pChild->isEnabled())
			{
				CCPoint local = pChild->convertToNodeSpace(touchLocation);
				CCRect r = pChild->rect();
				r.origin = CCPointZero;
				
				if (r.containsPoint(local))
				{
					return pChild;
				}
			}
		}
	}
	
	return NULL;
}


