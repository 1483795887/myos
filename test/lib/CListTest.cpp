#include "pch.h"
#include <lib/CList.h>
#include <types.h>

class CListTest : public testing::Test {
public:
    virtual void SetUp() {

    }
    virtual void TearDown() {

    }
};

TEST_F(CListTest, emptyListWhenGetCountThenCountZero) {
    CList list;
    EXPECT_EQ(list.getCount(), 0);
}

TEST_F(CListTest, emptyWhenInsertTailThenCountOne) {
    CList list;
    CListEntry entry;
    list.insertHead(&entry);
    EXPECT_EQ(list.getCount(), 1);
}

TEST_F(CListTest, nullWhenInsertTailThenCountZero) {
    CList list;
    list.insertHead(NULL);
    EXPECT_EQ(list.getCount(), 0);
}

TEST_F(CListTest, emptyWhenInsertTailThenFirstSelf) {
    CList list;
    EXPECT_EQ(list.getHead(), list.getFirst());
}

TEST_F(CListTest, afterInsertTailThenFirstOther) {
    CList list;
    CListEntry entry;
    list.insertHead(&entry);
    EXPECT_EQ(&entry, list.getFirst());
}

TEST_F(CListTest, twoWhenInsertTailThenThirdNextToSelf) {
    CList list;
    CListEntry entry, entry2;
    list.insertHead(&entry);
    list.insertHead(&entry2);
    CListEntry* first = list.getFirst();
    CListEntry* second = first->getNext();
    EXPECT_EQ(list.getHead(), second->getNext());
}

TEST_F(CListTest, twoWhenInsertTailThenFirstPrevToSelf) {
    CList list;
    CListEntry entry, entry2;
    list.insertHead(&entry);
    list.insertHead(&entry2);
    CListEntry* head = list.getHead();
    CListEntry* first = list.getFirst();
    EXPECT_EQ(list.getHead(), first->getPrev());
}

TEST_F(CListTest, oneWhenRemoveThenCountZero) {
    CList list;
    CListEntry entry;
    list.insertHead(&entry);
    list.remove(&entry);

    EXPECT_EQ(list.getCount(), 0);
}

TEST_F(CListTest, oneWhenRemoveThenFirstSelf) {
    CList list;
    CListEntry entry;
    list.insertHead(&entry);
    list.remove(&entry);

    EXPECT_EQ(list.getFirst(), list.getHead());
}

TEST_F(CListTest, twoWhenRemoveTheLaterThenNextToFormerAdded) {
    CList list;
    CListEntry entry, entry1;
    list.insertHead(&entry);
    list.insertHead(&entry1);
    list.remove(&entry1);

    EXPECT_EQ(list.getFirst(), &entry);
}

TEST_F(CListTest, notListEntryWhenRemoveThenNothingHappened) {
    CList list;
    CListEntry entry, entry1, entry2;
    list.insertHead(&entry);
    list.insertHead(&entry1);
    list.remove(&entry2);

    EXPECT_EQ(list.getCount(), 2);
}

TEST_F(CListTest, deleteDeletedWhenRemoveThenCountRight) {
    CList list;
    CListEntry entry, entry1;
    list.insertHead(&entry);
    list.insertHead(&entry1);
    list.remove(&entry1);
    list.remove(&entry1);

    EXPECT_EQ(list.getCount(), 1);
}

TEST_F(CListTest, emptyWhenRemoveThenNothingHappened) {
    CList list;
    list.remove(list.getHead());
    EXPECT_EQ(list.getCount(), 0);
}