package lr3_test

import (
	"os"
	"testing"

	"github.com/stretchr/testify/require"

	"lr3/lr3"
)

func TestSinglyListBasicOps(t *testing.T) {
	l := lr3.NewSinglyList()
	require.True(t, l.IsEmpty())
	require.Equal(t, 0, l.Size())

	l.PushBack("B")
	l.PushFront("A")
	l.PushBack("C")

	require.Equal(t, 3, l.Size())
	require.True(t, l.Search("A"))
	require.True(t, l.Search("B"))
	require.True(t, l.Search("C"))

	v, err := l.PopFront()
	require.NoError(t, err)
	require.Equal(t, "A", v)

	v, err = l.PopBack()
	require.NoError(t, err)
	require.Equal(t, "C", v)

	require.Equal(t, 1, l.Size())
}

func TestSinglyListInsertBeforeAfterAndRemove(t *testing.T) {
	l := lr3.NewSinglyList()
	l.PushBack("A")
	l.PushBack("C")

	ok := l.InsertAfter("A", "B")
	require.True(t, ok)
	ok = l.InsertBefore("A", "Start")
	require.True(t, ok)
	ok = l.InsertBefore("X", "Nope")
	require.False(t, ok)

	require.Equal(t, 4, l.Size())
	require.True(t, l.Search("Start"))
	require.True(t, l.Search("B"))

	ok = l.RemoveByValue("B")
	require.True(t, ok)
	ok = l.RemoveByValue("Z")
	require.False(t, ok)
}

func TestSinglyListPopOnEmpty(t *testing.T) {
	l := lr3.NewSinglyList()

	_, err := l.PopFront()
	require.Error(t, err)

	_, err = l.PopBack()
	require.Error(t, err)
}

func TestSinglyListClear(t *testing.T) {
	l := lr3.NewSinglyList()
	l.PushBack("1")
	l.PushBack("2")

	l.Clear()
	require.True(t, l.IsEmpty())
	require.Equal(t, 0, l.Size())
	require.False(t, l.Search("1"))
}

func TestSinglyListTextSerialize(t *testing.T) {
	path := "slist_txt_test.txt"
	defer os.Remove(path)

	l := lr3.NewSinglyList()
	l.PushBack("one")
	l.PushBack("two")
	require.NoError(t, l.SaveText(path))

	l2 := lr3.NewSinglyList()
	require.NoError(t, l2.LoadText(path))

	require.Equal(t, 2, l2.Size())
	require.True(t, l2.Search("one"))
	require.True(t, l2.Search("two"))
}

func TestSinglyListBinarySerialize(t *testing.T) {
	path := "slist_bin_test.bin"
	defer os.Remove(path)

	l := lr3.NewSinglyList()
	l.PushBack("x")
	l.PushBack("y")
	require.NoError(t, l.SaveBinary(path))

	l2 := lr3.NewSinglyList()
	require.NoError(t, l2.LoadBinary(path))

	require.Equal(t, 2, l2.Size())
	require.True(t, l2.Search("x"))
	require.True(t, l2.Search("y"))
}

func TestSinglyListErrorPaths(t *testing.T) {
	l := lr3.NewSinglyList()
	l.PushBack("val")

	err := l.SaveText("/root/slist_forbidden.txt")
	require.Error(t, err)

	err = l.SaveBinary("/root/slist_forbidden.bin")
	require.Error(t, err)

	err = l.LoadText("/root/slist_forbidden.txt")
	require.Error(t, err)

	err = l.LoadBinary("/root/slist_forbidden.bin")
	require.Error(t, err)
}

func TestSinglyListInsertFailBranches(t *testing.T) {
	l := lr3.NewSinglyList()

	// Insert* на пустом списке
	ok := l.InsertAfter("X", "A")
	require.False(t, ok)
	ok = l.InsertBefore("X", "A")
	require.False(t, ok)

	// Список без целевого элемента
	l.PushBack("1")
	l.PushBack("2")
	ok = l.InsertAfter("Z", "X")
	require.False(t, ok)
	ok = l.InsertBefore("Z", "Y")
	require.False(t, ok)

	require.Equal(t, 2, l.Size())
	require.True(t, l.Search("1"))
	require.True(t, l.Search("2"))
}

func TestSinglyListRemoveMiddleAndTail(t *testing.T) {
	l := lr3.NewSinglyList()
	l.PushBack("A")
	l.PushBack("B")
	l.PushBack("C")

	// Удаляем середину
	ok := l.RemoveByValue("B")
	require.True(t, ok)
	require.False(t, l.Search("B"))
	require.True(t, l.Search("A"))
	require.True(t, l.Search("C"))
	require.Equal(t, 2, l.Size())

	// Удаляем хвост
	ok = l.RemoveByValue("C")
	require.True(t, ok)
	require.False(t, l.Search("C"))
	require.True(t, l.Search("A"))
	require.Equal(t, 1, l.Size())
}

func TestSinglyListSerializeEmpty(t *testing.T) {
	txt := "slist_empty_txt.txt"
	bin := "slist_empty_bin.bin"
	defer os.Remove(txt)
	defer os.Remove(bin)

	l := lr3.NewSinglyList()

	// Пустой список в текст и бинарь
	require.NoError(t, l.SaveText(txt))
	require.NoError(t, l.SaveBinary(bin))

	l2 := lr3.NewSinglyList()
	require.NoError(t, l2.LoadText(txt))
	require.True(t, l2.IsEmpty())

	l3 := lr3.NewSinglyList()
	require.NoError(t, l3.LoadBinary(bin))
	require.True(t, l3.IsEmpty())
}

func TestSinglyListPopBackSingleElement(t *testing.T) {
	l := lr3.NewSinglyList()

	l.PushBack("only")

	v, err := l.PopBack()
	require.NoError(t, err)
	require.Equal(t, "only", v)

	require.True(t, l.IsEmpty())
}
