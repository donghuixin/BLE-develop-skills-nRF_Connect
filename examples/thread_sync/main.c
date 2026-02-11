/*
 * Nordic Academy — NCS Fundamentals Lessons 7 & 8
 * Example: Multithreading with Mutex synchronization
 *
 * Demonstrates:
 *   - K_THREAD_DEFINE for static thread creation
 *   - K_MUTEX_DEFINE for protecting shared resources
 *   - Race condition detection and prevention
 *   - Equal-priority threads with time slicing
 *
 * Board: Any NCS-supported board
 * Build: west build -b <board> -p always
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>

LOG_MODULE_REGISTER(thread_sync, LOG_LEVEL_INF);

#define STACKSIZE 1024
#define THREAD0_PRIORITY 4
#define THREAD1_PRIORITY 4

/* Shared resources */
#define COMBINED_TOTAL 40
static int32_t increment_count = 0;
static int32_t decrement_count = COMBINED_TOTAL;

/* Mutex to protect critical section */
K_MUTEX_DEFINE(data_mutex);

/* Semaphore for ISR → Thread signaling demo */
K_SEM_DEFINE(work_sem, 0, 1);

void shared_code_section(const char *caller) {
  int32_t inc_copy, dec_copy;

  /* Lock mutex before accessing shared data */
  k_mutex_lock(&data_mutex, K_FOREVER);

  increment_count = (increment_count + 1) % COMBINED_TOTAL;
  decrement_count -= 1;
  if (decrement_count == 0) {
    decrement_count = COMBINED_TOTAL;
  }

  inc_copy = increment_count;
  dec_copy = decrement_count;

  k_mutex_unlock(&data_mutex);

  /* Check invariant outside critical section */
  if (inc_copy + dec_copy != COMBINED_TOTAL) {
    LOG_ERR("RACE CONDITION in %s! %d + %d = %d (expected %d)", caller,
            inc_copy, dec_copy, inc_copy + dec_copy, COMBINED_TOTAL);
  }
}

void thread0(void) {
  LOG_INF("Thread 0 started (priority %d)", THREAD0_PRIORITY);
  while (1) {
    shared_code_section("Thread0");
    k_msleep(10);
  }
}

void thread1(void) {
  LOG_INF("Thread 1 started (priority %d)", THREAD1_PRIORITY);
  while (1) {
    shared_code_section("Thread1");
    k_msleep(10);
  }
}

K_THREAD_DEFINE(thread0_id, STACKSIZE, thread0, NULL, NULL, NULL,
                THREAD0_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread1_id, STACKSIZE, thread1, NULL, NULL, NULL,
                THREAD1_PRIORITY, 0, 0);

int main(void) {
  LOG_INF("=== NCS Fundamentals — Thread Sync Demo ===");
  LOG_INF("Two threads share counters protected by a mutex.");
  LOG_INF("If you remove the mutex, race conditions will occur.");

  while (1) {
    k_msleep(5000);
    LOG_INF("Heartbeat: inc=%d, dec=%d, sum=%d", increment_count,
            decrement_count, increment_count + decrement_count);
  }

  return 0;
}
