/*
 *  Copyright (C) 2017 KeePassXC Team <team@keepassxc.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KEEPASSXC_ASYNCTASK_HPP
#define KEEPASSXC_ASYNCTASK_HPP

#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QtDebug>

/**
 * Asynchronously run computations outside the GUI thread.
 */
namespace AsyncTask
{

    /**
     * Wait for the given future without blocking the event loop.
     *
     * @param future future to wait for
     * @return async task result
     */
    template <typename FunctionObject>
    typename std::result_of<FunctionObject()>::type
    waitForFuture(QFuture<typename std::result_of<FunctionObject()>::type> future)
    {
        QEventLoop loop;
        qDebug("WaitForFuture 1");
        QFutureWatcher<typename std::result_of<FunctionObject()>::type> watcher;
        qDebug("WaitForFuture 2");
        QObject::connect(&watcher, SIGNAL(finished()), &loop, SLOT(quit()));
        qDebug("WaitForFuture 3");
        watcher.setFuture(future);
        qDebug("WaitForFuture 4");
        loop.exec();
        qDebug("WaitForFuture 5");
        auto x = future.result();
        qDebug("WaitForFuture 6");
        qDebug() << "x: " << x;
        return x;
    }

    /**
     * Run a given task and wait for it to finish without blocking the event loop.
     *
     * @param task std::function object to run
     * @return async task result
     */
    template <typename FunctionObject>
    typename std::result_of<FunctionObject()>::type runAndWaitForFuture(FunctionObject task)
    {
        return waitForFuture<FunctionObject>(QtConcurrent::run(task));
    }

}; // namespace AsyncTask

#endif // KEEPASSXC_ASYNCTASK_HPP
