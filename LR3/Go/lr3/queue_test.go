package lr3_test

import (
	"os"
	"testing"

	"github.com/stretchr/testify/require"

	"lr3/lr3"
)

func TestQueueBasic(t *testing.T) {
	q := lr3.NewQueue()

	require.True(t, q.IsEmpty())
	require.Equal(t, 0, q.Size())

	q.Enqueue("a")
	q.Enqueue("b")
	q.Enqueue("c")

	require.False(t, q.IsEmpty())
	require.Equal(t, 3, q.Size())

	v, err := q.Peek()
	require.NoError(t, err)
	require.Equal(t, "a", v)

	v, err = q.Dequeue()
	require.NoError(t, err)
	require.Equal(t, "a", v)

	v, err = q.Dequeue()
	require.NoError(t, err)
	require.Equal(t, "b", v)

	v, err = q.Dequeue()
	require.NoError(t, err)
	require.Equal(t, "c", v)

	_, err = q.Dequeue()
	require.Error(t, err)
}

func TestQueueEmptyPeek(t *testing.T) {
	q := lr3.NewQueue()
	_, err := q.Peek()
	require.Error(t, err)
}

func TestQueueTextSerialize(t *testing.T) {
	path := "queue_txt_test.txt"
	defer os.Remove(path)

	q := lr3.NewQueue()
	q.Enqueue("one")
	q.Enqueue("two")
	require.NoError(t, q.SaveText(path))

	q2 := lr3.NewQueue()
	require.NoError(t, q2.LoadText(path))

	require.Equal(t, 2, q2.Size())
	v, _ := q2.Dequeue()
	require.Equal(t, "one", v)
	v, _ = q2.Dequeue()
	require.Equal(t, "two", v)
}

func TestQueueBinarySerialize(t *testing.T) {
	path := "queue_bin_test.bin"
	defer os.Remove(path)

	q := lr3.NewQueue()
	q.Enqueue("x")
	q.Enqueue("y")
	require.NoError(t, q.SaveBinary(path))

	q2 := lr3.NewQueue()
	require.NoError(t, q2.LoadBinary(path))

	require.Equal(t, 2, q2.Size())
	v, _ := q2.Dequeue()
	require.Equal(t, "x", v)
	v, _ = q2.Dequeue()
	require.Equal(t, "y", v)
}

func TestQueueErrorPaths(t *testing.T) {
	q := lr3.NewQueue()
	q.Enqueue("val")

	// Ошибки при создании/открытии файлов
	err := q.SaveText("/root/queue_forbidden.txt")
	require.Error(t, err)

	err = q.SaveBinary("/root/queue_forbidden.bin")
	require.Error(t, err)

	err = q.LoadText("/root/queue_forbidden.txt")
	require.Error(t, err)

	err = q.LoadBinary("/root/queue_forbidden.bin")
	require.Error(t, err)
}
