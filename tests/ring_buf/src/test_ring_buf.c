/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */

#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	/* TODO(l8-task1): Push a value, call rb_init(4) again, then
	 * verify the buffer is empty and count is 0.
	 * See TEST_SPEC.md "Suite ring_buf_init" #2.
	 */
	
	// Pushea un numero en el buffer
	zassert_ok(rb_push(99), "Push of a single value should succeed");

	// Reinicia el buffer otra vez
	zassert_ok(rb_init(4), "The reinit should succeed");

	// Se testea que el buffer esté vacío luego del reinicio
	zassert_true(rb_is_empty(), "The buffer should now be empty");

	// Se testea que count devuelva que la cantidad de elementos es 0 en el bufer
	zassert_equal(rb_count(), 0, "The buffer count must be 0");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	/* TODO(l8-task1): rb_push(42), rb_pop(&v) -> v == 42, buffer empty after.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #1.
	 */

	int v;

	// Se pushea un valor, se verifica que el push sea exitoso
	zassert_ok(rb_push(42), "Push of a single value should succeed");

	// Se popea un valor del buffer, se verifica que sea exitoso
	zassert_ok(rb_pop(&v), "Pop of a single value should succeed");

	// Se testea que el valor popeado coincida con el valor pusheado
	zassert_equal(v, 42, "Pop value should be the same as pushed value");

	// Se testea que el buffer quedó vacío después de popear el único valor pusheado
	zassert_true(rb_is_empty(), "The buffer should now be empty");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	/* TODO(l8-task1): rb_push(1), rb_push(2), rb_push(3); pop three times
	 * and verify the values come out as 1, 2, 3 in that order.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #2.
	 */
	int i;
	int v;

	// Se testea que se puedan pushear 3 números consecutivos
	for(i = 1; i < 3+1; i++) {
		zassert_ok(rb_push(i), "Push of value %d should succeed", i);

	// Se popean los 3 valores del buffer y se testea que sean correctos
	for(i = 3; i > 1-1; i--) {
		zassert_ok(rb_pop(&v), "Pop of value should succeed");
		zassert_equal(v, i, "Pop value should be %d", i);
	}

	// Finalmente, se testea que el buffer esté vacío luego de hacer los pops
	zassert_true(rb_is_empty(), "The buffer should now be empty");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	/* TODO(l8-task1): Fill the buffer to its capacity of 4, then push
	 * one more value -> -ENOSPC.
	 * See TEST_SPEC.md "Suite ring_buf_push_pop" #3.
	 */
	
	int i;

	// Se pusehan 4 valores en el buffer para llenarlo
	for(i = 1; i < 4+1; i++) {
		zassert_ok(rb_push(i), "Push of value %d should succeed", i);
	}

	// Se testea que el buffer está lleno
	zassert_true(rb_is_full(), "Buffer should be full");

	// Se intenta pushear un valor más y debería devolver un error
	zassert_equal(rb_push(99), -ENOSPC, "The push should be rejected as it is full by now");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
	/* TODO(l8-task1): rb_push(7); rb_peek(&v) -> v == 7; rb_peek(&v) again
	 * -> v == 7; rb_count() still == 1.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #1.
	 */

	int i;
	int v;
	
	// Se pushea un valor, se verifica que el push sea exitoso
	zassert_ok(rb_push(7), "Push of a single value should succeed");

	for(i = 0; i < 2; i++) {
		// Se hace un peek del buffer, se verifica que sea exitoso
		zassert_ok(rb_peek(&v), "Peek number %d should succeed", i);

		// El valor debe ser el mismo que fue pusheado antes
		zassert_equal(v, 7, "Peek number %d: value should be 7, but it is %d", i, v);
	}

	// Se verifica que haber hecho peek no popea el elemento del buffer
	zassert_equal(rb_count(), 1, "Peek should not pop elements of the buffer");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	/* TODO(l8-task1): rb_pop(NULL) -> -EINVAL.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #2.
	 */
	
	// Se intenta almacenar el valor poppeado en la dirección de un NULL pointer, debería retornar -EINVAL
	zassert_equal(rb_pop(NULL), -EINVAL, "Popping to a NULL pointer should return -EINVAL");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	/* TODO(l8-task1): push 4 values -> rb_is_full() == true, rb_count() == 4.
	 * See TEST_SPEC.md "Suite ring_buf_boundaries" #3.
	 */
	
	int i;

	// Se pusehan 4 valores en el buffer para llenarlo
	for(i = 1; i < 4+1; i++) {
		zassert_ok(rb_push(i), "Push of value %d should succeed", i);
	}

	// Después de pushear, el buffer debería estar lleno
	zassert_true(rb_is_full(), "Buffer should be full after filling it completely");

	// Se verifica que la cantidad de elementos del buffer sea 4
	zassert_equal(rb_count(), 4, "Buffer count should be 4");
}
