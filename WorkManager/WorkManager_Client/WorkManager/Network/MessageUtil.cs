﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace Protocol
{
    public class MessageUtil
    {
        public static void Send(Stream writer, Message Msg)
        {
            writer.Write(Msg.GetBytes(), 0, Msg.GetSize());
        }

        public static Message Receive(Stream reader)
        {
            int n_total_recv = 0;
            int n_size_to_read = 16;
            byte[] hBuffer = new byte[n_size_to_read];

            while (n_size_to_read > 0)
            {
                byte[] buffer = new byte[n_size_to_read];
                int n_recv = reader.Read(buffer, 0, n_size_to_read);

                if (n_recv == 0)
                {
                    return null;
                }

                buffer.CopyTo(hBuffer, n_total_recv);
                n_total_recv += n_recv;
                n_size_to_read -= n_recv;
            }

            Header header = new Header(hBuffer);

            n_total_recv = 0;
            byte[] bBuffer = new byte[header.BODYLEN];
            n_size_to_read = (int)header.BODYLEN;

            while (n_size_to_read > 0)
            {
                byte[] buffer = new byte[n_size_to_read];
                int n_recv = reader.Read(buffer, 0, n_size_to_read);

                if (n_recv == 0)
                {
                    return null;
                }

                buffer.CopyTo(bBuffer, n_total_recv);
                n_total_recv += n_recv;
                n_size_to_read -= n_recv;
            }

            ISerializable body = null;

            switch (header.MSGTYPE)
            {
                case CONSTANTS.REQ_FILE_SEND:
                case CONSTANTS.REQ_FILE_RECEIVE:
                    body = new BodyRequest(bBuffer);
                    break;
                case CONSTANTS.REP_FILE_SEND:
                case CONSTANTS.REP_FILE_RECEIVE:
                    body = new BodyResponse(bBuffer);
                    break;
                case CONSTANTS.FILE_SEND_DATA:
                case CONSTANTS.FILE_RECEIVE_DATA:
                    body = new BodyData(bBuffer);
                    break;
                case CONSTANTS.FILE_SEND_RES:
                case CONSTANTS.FILE_RECEIVE_RES:
                    body = new BodyResult(bBuffer);
                    break;
                default:
                    throw new Exception(string.Format("Unknown MSGTYPE: {0}", header.MSGTYPE));
            }

            return new Message() { Header = header, Body = body };
        }
    }
}
