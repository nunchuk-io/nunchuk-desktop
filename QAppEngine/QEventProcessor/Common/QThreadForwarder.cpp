/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/
#include "QThreadForwarder.h"
#include "Chats/ClientController.h"
#include "WorkerThread.h"

WorkerThread *WorkerThread::instance() {
    static WorkerThread mInstance;
    return &mInstance;
}

WorkerThread::WorkerThread() {}

WorkerThread::~WorkerThread() { // Correct destructor syntax
    {
        QMutexLocker locker(&mutex);
        stopThread = true;   // Signal the thread to stop
        condition.wakeOne(); // Wake up the thread so it can exit
    }
    wait(); // Ensure thread stops before destruction
}

void WorkerThread::runTask(Task task) {
    if (!isRunning()) {
        stopThread = false;
        start(); // Start worker thread on creation
    }
    QMutexLocker locker(&mutex);
    taskQueue.enqueue(task);
    condition.wakeOne(); // Wake up the worker thread
}

void WorkerThread::run() {
    while (!stopThread) { // Exit loop when stopping
        if (!ClientController::instance()->isNunchukLoggedIn()) {
            QThread::msleep(100);  // Sleep to avoid busy waiting
            break; // Exit loop if not logged in
        }
        Task task;
        {
            QMutexLocker locker(&mutex);
            while (taskQueue.isEmpty() && !stopThread) { // Prevent CPU spinning
                condition.wait(&mutex);                  // Wait for new tasks
            }
            if (stopThread)
                break; // Exit loop when stopping
            if (!taskQueue.isEmpty()) {
                task = taskQueue.dequeue(); // Get next task
            }
        }
        if (task) {
            try {
                task(); // Execute task
            } catch (const std::exception &e) {
                DBG_ERROR << "Task execution failed: " << e.what();
            }
        }
    }
}