package lr3_test

import (
	"os"
	"testing"

	"github.com/stretchr/testify/require"

	"lr3/lr3"
)

func TestDoublyListBasicOps(t *testing.T) {
	l := lr3.NewDoublyList()
	require.True(t, l.IsEmpty())
	require.Equal(t, 0, l.Size())

	l.PushBack("B")
	l.PushFront("A")
	l.PushBack("C")

	require.Equal(t, 3, l.Size())

	v, err := l.PopFront()
	require.NoError(t, err)
	require.Equal(t, "A", v)

	v, err = l.PopBack()
	require.NoError(t, err)
	require.Equal(t, "C", v)

	require.Equal(t, 1, l.Size())
}

func TestDoublyListPopSingleElement(t *testing.T) {
	l := lr3.NewDoublyList()
	l.PushBack("only")

	v, err := l.PopBack()
	require.NoError(t, err)
	require.Equal(t, "only", v)
	require.True(t, l.IsEmpty())

	l.PushFront("one")
	v, err = l.PopFront()
	require.NoError(t, err)
	require.Equal(t, "one", v)
	require.True(t, l.IsEmpty())
}

func TestDoublyListPopOnEmpty(t *testing.T) {
	l := lr3.NewDoublyList()

	_, err := l.PopFront()
	require.Error(t, err)

	_, err = l.PopBack()
	require.Error(t, err)
}

func TestDoublyListInsertAfterBeforeAndRemove(t *testing.T) {
	l := lr3.NewDoublyList()
	l.PushBack("A")
	l.PushBack("C")

	ok := l.InsertAfter("A", "B")
	require.True(t, ok)

	ok = l.InsertBefore("A", "Start")
	require.True(t, ok)

	ok = l.InsertBefore("X", "Nope")
	require.False(t, ok)

	ok = l.InsertAfter("Z", "Nope2")
	require.False(t, ok)

	require.Equal(t, 4, l.Size())

	// Удаляем середину
	ok = l.RemoveByValue("B")
	require.True(t, ok)

	// Удаляем несуществующий
	ok = l.RemoveByValue("Z")
	require.False(t, ok)
}

func TestDoublyListRemoveHeadAndTail(t *testing.T) {
	l := lr3.NewDoublyList()
	l.PushBack("A")
	l.PushBack("B")
	l.PushBack("C")

	// Удаляем голову через RemoveByValue
	ok := l.RemoveByValue("A")
	require.True(t, ok)
	require.Equal(t, 2, l.Size())

	// Удаляем хвост через RemoveByValue
	ok = l.RemoveByValue("C")
	require.True(t, ok)
	require.Equal(t, 1, l.Size())

	// Остался только "B"
	v, err := l.PopFront()
	require.NoError(t, err)
	require.Equal(t, "B", v)
	require.True(t, l.IsEmpty())
}

func TestDoublyListInsertBeforeHeadOnly(t *testing.T) {
	l := lr3.NewDoublyList()
	l.PushBack("Head")

	ok := l.InsertBefore("Head", "NewHead")
	require.True(t, ok)
	require.Equal(t, 2, l.Size())

	v, err := l.PopFront()
	require.NoError(t, err)
	require.Equal(t, "NewHead", v)
}

func TestDoublyListClear(t *testing.T) {
	l := lr3.NewDoublyList()
	l.PushBack("1")
	l.PushBack("2")

	l.Clear()
	require.True(t, l.IsEmpty())
	require.Equal(t, 0, l.Size())
}

func TestDoublyListTextSerialize(t *testing.T) {
	path := "dlist_txt_test.txt"
	defer os.Remove(path)

	l := lr3.NewDoublyList()
	l.PushBack("one")
	l.PushBack("two")
	require.NoError(t, l.SaveText(path))

	l2 := lr3.NewDoublyList()
	require.NoError(t, l2.LoadText(path))

	require.Equal(t, 2, l2.Size())
}

func TestDoublyListBinarySerialize(t *testing.T) {
	path := "dlist_bin_test.bin"
	defer os.Remove(path)

	l := lr3.NewDoublyList()
	l.PushBack("x")
	l.PushBack("y")
	require.NoError(t, l.SaveBinary(path))

	l2 := lr3.NewDoublyList()
	require.NoError(t, l2.LoadBinary(path))

	require.Equal(t, 2, l2.Size())
}

func TestDoublyListSerializeEmpty(t *testing.T) {
	txt := "dlist_empty_txt.txt"
	bin := "dlist_empty_bin.bin"
	defer os.Remove(txt)
	defer os.Remove(bin)

	l := lr3.NewDoublyList()

	require.NoError(t, l.SaveText(txt))
	require.NoError(t, l.SaveBinary(bin))

	l2 := lr3.NewDoublyList()
	require.NoError(t, l2.LoadText(txt))
	require.True(t, l2.IsEmpty())

	l3 := lr3.NewDoublyList()
	require.NoError(t, l3.LoadBinary(bin))
	require.True(t, l3.IsEmpty())
}

func TestDoublyListErrorPaths(t *testing.T) {
	l := lr3.NewDoublyList()
	l.PushBack("val")

	err := l.SaveText("/root/dlist_forbidden.txt")
	require.Error(t, err)

	err = l.SaveBinary("/root/dlist_forbidden.bin")
	require.Error(t, err)

	err = l.LoadText("/root/dlist_forbidden.txt")
	require.Error(t, err)

	err = l.LoadBinary("/root/dlist_forbidden.bin")
	require.Error(t, err)
}
