/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_CIRCULARBUFFER_H
#define MBED_CIRCULARBUFFER_H

namespace mbed {

/** Templated Circular buffer class
 */
template<typename T, uint32_t BufferSize, typename CounterType = uint32_t>
class CircularBuffer {
public:
    CircularBuffer() : _head(0), _tail(0), _full(false) {
    }

    ~CircularBuffer() {
    }

    /** Push the transaction to the queue. This overwrites the buffer if it's
     *  full, therefore use full method not to overwrite
     *
     * @param data Data to be pushed to the queue
     */
    void push(const T& data) {
        if (full()) {
            _tail++;
            _tail %= BufferSize;
        }
        _pool[_head++] = data;
        _head %= BufferSize;
        if (_head == _tail) {
            _full = true;
        }
    }

    /** Pop the transaction from the queue
     *
     * @param data Data to be pushed to the queue
     * @return True if the queue is not empty and data contains a transaction, false otherwise
     */
    bool pop(T& data) {
        if (!empty()) {
            data = _pool[_tail++];
            _tail %= BufferSize;
            _full = false;
            return true;
        }
        return false;
    }

    /** Checks if the queue is empty
     *
     * @return True if the queue is empty, false if not
     */
    bool empty() {
        return (_head == _tail) && !_full;
    }

    /** Checks if the queue is full
     *
     * @return True if the queue is full, false if not
     */
    bool full() {
        return  (_head == _tail) && _full;
    }

private:
    T _pool[BufferSize];
    CounterType _head;
    CounterType _tail;
    bool _full;
};

}

#endif
