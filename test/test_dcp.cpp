/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2018 rt-labs AB, Sweden.
 *
 * This software is dual-licensed under GPLv3 and a commercial
 * license. See the file LICENSE.md distributed with this software for
 * full license information.
 ********************************************************************/

/**
 * @file
 * @brief Integration testing of DCP aspects.
 *
 * For example
 *   Sending hello frame
 *   Handling of get name request
 *   Read station name
 *   Set station name
 *   Set IP address
 *   Do factory reset
 *
 * Checks only function return codes. No checking of the sent Ethernet frames is
 * done.
 *
 */

#include "utils_for_testing.h"
#include "mocks.h"

#include "pf_includes.h"

#include <gtest/gtest.h>

class DcpTest : public PnetIntegrationTest
{
};

static uint8_t get_name_req[] = {
   0x1e, 0x30, 0x6c, 0xa2, 0x45, 0x5e, 0xc8, 0x5b, 0x76, 0xe6, 0x89, 0xdf,
   0x88, 0x92, 0xfe, 0xfd, 0x03, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00,
   0x00, 0x06, 0x02, 0x02, 0x02, 0x03, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static uint8_t ident_req[] = {
   0x01, 0x0e, 0xcf, 0x00, 0x00, 0x00, 0xc8, 0x5b, 0x76, 0xe6, 0x89, 0xdf,
   0x88, 0x92, 0xfe, 0xfe, 0x05, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01,
   0x00, 0x10, 0x02, 0x02, 0x00, 0x0c, 0x72, 0x74, 0x2d, 0x6c, 0x61, 0x62,
   0x73, 0x2d, 0x64, 0x65, 0x6d, 0x6f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static uint8_t set_name_req[] = {
   0x1e, 0x30, 0x6c, 0xa2, 0x45, 0x5e, 0xc8, 0x5b, 0x76, 0xe6, 0x89, 0xdf,
   0x88, 0x92, 0xfe, 0xfd, 0x04, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
   0x00, 0x12, 0x02, 0x02, 0x00, 0x0e, 0x00, 0x00, 0x72, 0x74, 0x2d, 0x6c,
   0x61, 0x62, 0x73, 0x2d, 0x64, 0x65, 0x6d, 0x6f, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static uint8_t set_ip_req[] = {
   0x1e, 0x30, 0x6c, 0xa2, 0x45, 0x5e, 0xc8, 0x5b, 0x76, 0xe6, 0x89, 0xdf,
   0x88, 0x92, 0xfe, 0xfd, 0x04, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
   0x00, 0x18, 0x01, 0x02, 0x00, 0x0e, 0x00, 0x00, 0xc0, 0xa8, 0x01, 0xab,
   0xff, 0xff, 0xff, 0x00, 0xc0, 0xa8, 0x01, 0x01, 0x05, 0x02, 0x00, 0x02,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static uint8_t factory_reset_req[] = {
   0x1e, 0x30, 0x6c, 0xa2, 0x45, 0x5e, 0xc8, 0x5b, 0x76, 0xe6, 0x89, 0xdf,
   0x88, 0x92, 0xfe, 0xfd, 0x04, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00,
   0x00, 0x06, 0x05, 0x05, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static uint8_t signal_req[] = {
   0x1e, 0x30, 0x6c, 0xa2, 0x45, 0x5e, 0xc8, 0x5b, 0x76, 0xe6, 0x89, 0xdf,
   0x88, 0x92, 0xfe, 0xfd, 0x04, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00,
   0x00, 0x06, 0x05, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

TEST_F (DcpTest, DcpHelloTest)
{
   os_buf_t * p_buf;
   int ret;

   mock_clear();

   ret = pf_dcp_hello_req (net);
   EXPECT_EQ (ret, 0);

   mock_clear();

   p_buf = os_buf_alloc (PF_FRAME_BUFFER_SIZE);
   memcpy (p_buf->payload, get_name_req, sizeof (get_name_req));
   ret = pf_eth_recv (net, p_buf);

   EXPECT_EQ (ret, 1);
   EXPECT_EQ (mock_os_data.eth_send_count, 2);

   EXPECT_EQ (appdata.call_counters.led_off_calls, 1);
   EXPECT_EQ (appdata.call_counters.led_on_calls, 0);
   EXPECT_EQ (appdata.call_counters.state_calls, 0);
   EXPECT_EQ (appdata.call_counters.connect_calls, 0);
   EXPECT_EQ (appdata.call_counters.release_calls, 0);
   EXPECT_EQ (appdata.call_counters.dcontrol_calls, 0);
   EXPECT_EQ (appdata.call_counters.ccontrol_calls, 0);
   EXPECT_EQ (appdata.call_counters.read_calls, 0);
   EXPECT_EQ (appdata.call_counters.write_calls, 0);
}

TEST_F (DcpTest, DcpRunTest)
{
   os_buf_t * p_buf;
   int ret;

   TEST_TRACE ("\nGenerating mock set name request\n");
   p_buf = os_buf_alloc (PF_FRAME_BUFFER_SIZE);
   memcpy (p_buf->payload, set_name_req, sizeof (set_name_req));
   ret = pf_eth_recv (net, p_buf);
   EXPECT_EQ (ret, 1);

   TEST_TRACE ("\nGenerating mock set IP request\n");
   p_buf = os_buf_alloc (PF_FRAME_BUFFER_SIZE);
   memcpy (p_buf->payload, set_ip_req, sizeof (set_ip_req));
   ret = pf_eth_recv (net, p_buf);
   EXPECT_EQ (ret, 1);

   TEST_TRACE ("\nGenerating mock set ident request\n");
   p_buf = os_buf_alloc (PF_FRAME_BUFFER_SIZE);
   memcpy (p_buf->payload, ident_req, sizeof (ident_req));
   ret = pf_eth_recv (net, p_buf);
   EXPECT_EQ (ret, 1);

   TEST_TRACE ("\nGenerating mock factory reset request\n");
   p_buf = os_buf_alloc (PF_FRAME_BUFFER_SIZE);
   memcpy (p_buf->payload, factory_reset_req, sizeof (factory_reset_req));
   ret = pf_eth_recv (net, p_buf);
   EXPECT_EQ (ret, 1);

   TEST_TRACE ("\nGenerating mock flash LED request\n");
   p_buf = os_buf_alloc (PF_FRAME_BUFFER_SIZE);
   memcpy (p_buf->payload, signal_req, sizeof (signal_req));
   ret = pf_eth_recv (net, p_buf);
   EXPECT_EQ (ret, 1);
   /* Wait for LED to flash three times at 1 Hz */
   run_stack (4 * 1000 * 1000);

   EXPECT_EQ (mock_os_data.eth_send_count, 9);
   EXPECT_EQ (mock_os_data.set_ip_suite_count, 2);

   EXPECT_EQ (appdata.call_counters.led_on_calls, 3);
   EXPECT_EQ (appdata.call_counters.led_off_calls, 4);
   EXPECT_EQ (appdata.call_counters.state_calls, 0);
   EXPECT_EQ (appdata.call_counters.connect_calls, 0);
   EXPECT_EQ (appdata.call_counters.release_calls, 0);
   EXPECT_EQ (appdata.call_counters.dcontrol_calls, 0);
   EXPECT_EQ (appdata.call_counters.ccontrol_calls, 0);
   EXPECT_EQ (appdata.call_counters.read_calls, 0);
   EXPECT_EQ (appdata.call_counters.write_calls, 0);
}
