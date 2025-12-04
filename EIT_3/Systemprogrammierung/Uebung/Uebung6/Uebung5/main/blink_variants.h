#ifndef BLINK_VARIANTS_H
#define BLINK_VARIANTS_H

/* Aufgabe 3: Drei Varianten der Task-Kommunikation */

/* Variante 1: Producer/Consumer mit Semaphor */
void run_producer_consumer_semaphore(void);

/* Variante 2: Event Notification mit xTaskNotify */
void run_event_notification_variant(void);

/* Variante 3: Queue mit KeyEvent Struktur */
void run_queue_variant(void);

#endif // BLINK_VARIANTS_H
