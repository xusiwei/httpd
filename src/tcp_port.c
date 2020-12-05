/*
 * Copyright (c) 2020, HiHope Community.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "lwip/tcp.h"

err_t
tcp_tcp_get_tcp_addrinfo(struct tcp_pcb *pcb, int local, ip_addr_t *addr, u16_t *port)
{
  if (pcb) {
    if (local) {
      if (addr) {
        *addr = pcb->local_ip;
      }
      if (port) {
        *port = pcb->local_port;
      }
    } else {
      if (addr) {
        *addr = pcb->remote_ip;
      }
      if (port) {
        *port = pcb->remote_port;
      }
    }
    return ERR_OK;
  }
  return ERR_VAL;
}

/**
 * @ingroup tcp
 * Sets the priority of a connection.
 *
 * @param pcb the tcp_pcb to manipulate
 * @param prio new priority
 */
void
tcp_setprio(struct tcp_pcb *pcb, u8_t prio)
{
  LWIP_ASSERT_CORE_LOCKED();

  LWIP_ERROR("tcp_setprio: invalid pcb", pcb != NULL, return);

  pcb->prio = prio;
}

/**
 * @ingroup tcp_raw
 * Set the state of the connection to be LISTEN, which means that it
 * is able to accept incoming connections. The protocol control block
 * is reallocated in order to consume less memory. Setting the
 * connection to LISTEN is an irreversible process.
 * When an incoming connection is accepted, the function specified with
 * the tcp_accept() function will be called. The pcb has to be bound
 * to a local port with the tcp_bind() function.
 *
 * The tcp_listen() function returns a new connection identifier, and
 * the one passed as an argument to the function will be
 * deallocated. The reason for this behavior is that less memory is
 * needed for a connection that is listening, so tcp_listen() will
 * reclaim the memory needed for the original connection and allocate a
 * new smaller memory block for the listening connection.
 *
 * tcp_listen() may return NULL if no memory was available for the
 * listening connection. If so, the memory associated with the pcb
 * passed as an argument to tcp_listen() will not be deallocated.
 *
 * The backlog limits the number of outstanding connections
 * in the listen queue to the value specified by the backlog argument.
 * To use it, your need to set TCP_LISTEN_BACKLOG=1 in your lwipopts.h.
 *
 * @param pcb the original tcp_pcb
 * @param backlog the incoming connections queue limit
 * @return tcp_pcb used for listening, consumes less memory.
 *
 * @note The original tcp_pcb is freed. This function therefore has to be
 *       called like this:
 *             tpcb = tcp_listen_with_backlog(tpcb, backlog);
 */
struct tcp_pcb *
tcp_listen_with_backlog(struct tcp_pcb *pcb, u8_t backlog)
{
  LWIP_ASSERT_CORE_LOCKED();
  return tcp_listen_with_backlog_and_err(pcb, backlog, NULL);
}
