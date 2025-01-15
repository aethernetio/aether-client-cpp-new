/*
 * Copyright 2024 Aethernet Inc.
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

#ifndef AETHER_TELE_TELE_H_
#define AETHER_TELE_TELE_H_

#include "aether/tele/modules.h"
#include "aether/tele/configs/config_selector.h"

// redefine this macro to use your own sink
#ifndef TELE_SINK
#  define TELE_SINK SELECTED_SINK
#endif

// redefine this macro to use your own tag list
#ifndef TAG_LIST_NAME
#  define TAG_LIST_NAME ae::tele::tele_tag_list
#endif

#include "aether/tele/defines.h"
#include "aether/tele/register.h"

namespace ae::tele {
// register new tags here
inline constexpr auto tele_tag_list = ae::tele::Registration(  //
    AE_TAG("Object", Module::kObj),                            //
    AE_TAG("Started", Module::kApp),                           //
    AE_TAG("Registration Started", Module::kRegister),         //
    AE_TAG("Client registered", Module::kRegister),            //
    AE_TAG("Sending pull request", Module::kPull),             //
    AE_TAG("TcpTransport", Module::kTransport),                //
    AE_TAG("TcpTransportConnect", Module::kTransport),         //
    AE_TAG("TcpTransportDisconnect", Module::kTransport),      //
    AE_TAG("TcpTransportSend", Module::kTransport),            //
    AE_TAG("TcpTransportReceive", Module::kTransport),         //
    AE_TAG("TcpTransportOnData", Module::kTransport),          //
    AE_TAG("TcpTransportOnPacket", Module::kTransport),        //
    AE_TAG("Simulation", Module::kSim),                        //
    AE_TAG("LOG", Module::kLog)                                //
);
}  // namespace ae::tele

#endif  // AETHER_TELE_TELE_H_
