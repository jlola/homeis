/*
 * dis.cpp
 *
 *  Created on: Jan 22, 2013
 *      Author: root
 */

#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>   /* File control definitions */
//#include <libxml++/libxml++.h>
#include <libxml/parser.h>


#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#if defined(LIBXML_TREE_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)

/*
 *To compile this file using gcc you can type
 *gcc `xml2-config --cflags --libs` -o tree2 tree2.c
 */

/* A simple example how to create DOM. Libxml2 automagically
 * allocates the necessary amount of memory to it.
*/
int main5(int argc, char **argv)
{
//    xmlDocPtr doc = NULL;       /* document pointer */
//    xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
//    xmlDtdPtr dtd = NULL;       /* DTD pointer */
//    char buff[256];
//    int i, j;
//
//    LIBXML_TEST_VERSION;
//
//    /*
//     * Creates a new document, a node and set it as a root node
//     */
//    doc = xmlNewDoc(BAD_CAST "1.0");
//    root_node = xmlNewNode(NULL, BAD_CAST "root");
//    xmlDocSetRootElement(doc, root_node);
//
//    /*
//     * Creates a DTD declaration. Isn't mandatory.
//     */
//    dtd = xmlCreateIntSubset(doc, BAD_CAST "root", NULL, BAD_CAST "tree2.dtd");
//
//    /*
//     * xmlNewChild() creates a new node, which is "attached" as child node
//     * of root_node node.
//     */
//    xmlNewChild(root_node, NULL, BAD_CAST "node1",
//                BAD_CAST "content of node 1");
//    /*
//     * The same as above, but the new child node doesn't have a content
//     */
//    xmlNewChild(root_node, NULL, BAD_CAST "node2", NULL);
//
//    /*
//     * xmlNewProp() creates attributes, which is "attached" to an node.
//     * It returns xmlAttrPtr, which isn't used here.
//     */
//    node =
//        xmlNewChild(root_node, NULL, BAD_CAST "node3",
//                    BAD_CAST "this node has attributes");
//    xmlNewProp(node, BAD_CAST "attribute", BAD_CAST "yes");
//    xmlNewProp(node, BAD_CAST "foo", BAD_CAST "bar");
//
//    /*
//     * Here goes another way to create nodes. xmlNewNode() and xmlNewText
//     * creates a node and a text node separately. They are "attached"
//     * by xmlAddChild()
//     */
//    node = xmlNewNode(NULL, BAD_CAST "node4");
//    node1 = xmlNewText(BAD_CAST
//                   "other way to create content (which is also a node)");
//    xmlAddChild(node, node1);
//    xmlAddChild(root_node, node);
//
//    /*
//     * A simple loop that "automates" nodes creation
//     */
//    for (i = 5; i < 7; i++) {
//        sprintf(buff, "node%d", i);
//        node = xmlNewChild(root_node, NULL, BAD_CAST buff, NULL);
//        for (j = 1; j < 4; j++) {
//            sprintf(buff, "node%d%d", i, j);
//            node1 = xmlNewChild(node, NULL, BAD_CAST buff, NULL);
//            xmlNewProp(node1, BAD_CAST "odd", BAD_CAST((j % 2) ? "no" : "yes"));
//        }
//    }
//
//    /*
//     * Dumping document to stdio or file
//     */
//    int bytes = xmlSaveFormatFileEnc(/*argc > 1 ? argv[1] : "-"*/"testxml.xml", doc, "UTF-8", 1);
//    printf("saved %d bytes",bytes);
//
//
//    /*free the document */
//    xmlFreeDoc(doc);
//
//    /*
//     *Free the global variables that may
//     *have been allocated by the parser.
//     */
//    xmlCleanupParser();
//
//    /*
//     * this is to debug memory for regression tests
//     */
//    xmlMemoryDump();
    return(0);
}
#else
int main(void) {
    fprintf(stderr, "tree support not compiled in\n");
    exit(1);
}
#endif

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netdb.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <sys/epoll.h>
//#include <errno.h>
//
//#define MAXEVENTS 64
//
//static int
//make_socket_non_blocking (int sfd)
//{
//  int flags, s;
//
//  flags = fcntl (sfd, F_GETFL, 0);
//  if (flags == -1)
//    {
//      perror ("fcntl");
//      return -1;
//    }
//
//  flags |= O_NONBLOCK;
//  s = fcntl (sfd, F_SETFL, flags);
//  if (s == -1)
//    {
//      perror ("fcntl");
//      return -1;
//    }
//
//  return 0;
//}
//
//static int
//create_and_bind (char *port)
//{
//  struct addrinfo hints;
//  struct addrinfo *result, *rp;
//  int s, sfd;
//
//  memset (&hints, 0, sizeof (struct addrinfo));
//  hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
//  hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
//  hints.ai_flags = AI_PASSIVE;     /* All interfaces */
//
//  s = getaddrinfo (NULL, port, &hints, &result);
//  if (s != 0)
//    {
//      fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
//      return -1;
//    }
//
//  for (rp = result; rp != NULL; rp = rp->ai_next)
//    {
//      sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
//      if (sfd == -1)
//        continue;
//
//      s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
//      if (s == 0)
//        {
//          /* We managed to bind successfully! */
//          break;
//        }
//
//      close (sfd);
//    }
//
//  if (rp == NULL)
//    {
//      fprintf (stderr, "Could not bind\n");
//      return -1;
//    }
//
//  freeaddrinfo (result);
//
//  return sfd;
//}
//
//int
//main5 (int argc, char *argv[])
//{
//  int sfd, s;
//  int efd;
//  struct epoll_event event;
//  struct epoll_event *events;
//
////  if (argc != 2)
////    {
////      fprintf (stderr, "Usage: %s [port]\n", argv[0]);
////      exit (EXIT_FAILURE);
////    }
//
//  sfd = create_and_bind ("10000"/*argv[1]*/);
//  if (sfd == -1)
//    abort ();
//
//  s = make_socket_non_blocking (sfd);
//  if (s == -1)
//    abort ();
//
//  s = listen (sfd, SOMAXCONN);
//  if (s == -1)
//    {
//      perror ("listen");
//      abort ();
//    }
//
//  efd = epoll_create1 (0);
//  if (efd == -1)
//    {
//      perror ("epoll_create");
//      abort ();
//    }
//
//  event.data.fd = sfd;
//  event.events = EPOLLIN | EPOLLET;
//  s = epoll_ctl (efd, EPOLL_CTL_ADD, sfd, &event);
//  if (s == -1)
//    {
//      perror ("epoll_ctl");
//      abort ();
//    }
//
//  /* Buffer where events are returned */
//  events = (epoll_event*)calloc (MAXEVENTS, sizeof event);
//
//  /* The event loop */
//  while (1)
//    {
//      int n, i;
//
//      n = epoll_wait (efd, events, MAXEVENTS, -1);
//      for (i = 0; i < n; i++)
//	{
//	  if ((events[i].events & EPOLLERR) ||
//              (events[i].events & EPOLLHUP) ||
//              (!(events[i].events & EPOLLIN)))
//	    {
//              /* An error has occured on this fd, or the socket is not
//                 ready for reading (why were we notified then?) */
//	      fprintf (stderr, "epoll error\n");
//	      close (events[i].data.fd);
//	      continue;
//	    }
//
//	  else if (sfd == events[i].data.fd)
//	    {
//              /* We have a notification on the listening socket, which
//                 means one or more incoming connections. */
//              while (1)
//                {
//                  struct sockaddr in_addr;
//                  socklen_t in_len;
//                  int infd;
//                  char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
//
//                  in_len = sizeof in_addr;
//                  infd = accept (sfd, &in_addr, &in_len);
//                  if (infd == -1)
//                    {
//                      if ((errno == EAGAIN) ||
//                          (errno == EWOULDBLOCK))
//                        {
//                          /* We have processed all incoming
//                             connections. */
//                          break;
//                        }
//                      else
//                        {
//                          perror ("accept");
//                          break;
//                        }
//                    }
//
//                  s = getnameinfo (&in_addr, in_len,
//                                   hbuf, sizeof hbuf,
//                                   sbuf, sizeof sbuf,
//                                   NI_NUMERICHOST | NI_NUMERICSERV);
//                  if (s == 0)
//                    {
//                      printf("Accepted connection on descriptor %d "
//                             "(host=%s, port=%s)\n", infd, hbuf, sbuf);
//                    }
//
//                  /* Make the incoming socket non-blocking and add it to the
//                     list of fds to monitor. */
//                  s = make_socket_non_blocking (infd);
//                  if (s == -1)
//                    abort ();
//
//                  event.data.fd = infd;
//                  event.events = EPOLLIN | EPOLLET;
//                  s = epoll_ctl (efd, EPOLL_CTL_ADD, infd, &event);
//                  if (s == -1)
//                    {
//                      perror ("epoll_ctl");
//                      abort ();
//                    }
//                }
//              continue;
//            }
//          else
//            {
//              /* We have data on the fd waiting to be read. Read and
//                 display it. We must read whatever data is available
//                 completely, as we are running in edge-triggered mode
//                 and won't get a notification again for the same
//                 data. */
//              int done = 0;
//
//              while (1)
//                {
//                  ssize_t count;
//                  char buf[512];
//
//                  count = read (events[i].data.fd, buf, sizeof buf);
//                  if (count == -1)
//                    {
//                      /* If errno == EAGAIN, that means we have read all
//                         data. So go back to the main loop. */
//                      if (errno != EAGAIN)
//                        {
//                          perror ("read");
//                          done = 1;
//                        }
//                      break;
//                    }
//                  else if (count == 0)
//                    {
//                      /* End of file. The remote has closed the
//                         connection. */
//                      done = 1;
//                      break;
//                    }
//
//                  /* Write the buffer to standard output */
//                  s = write (1, buf, count);
//                  if (s == -1)
//                    {
//                      perror ("write");
//                      abort ();
//                    }
//                }
//
//              if (done)
//                {
//                  printf ("Closed connection on descriptor %d\n",
//                          events[i].data.fd);
//
//                  /* Closing the descriptor will make epoll remove it
//                     from the set of descriptors which are monitored. */
//                  close (events[i].data.fd);
//                }
//            }
//        }
//    }
//
//  free (events);
//
//  close (sfd);
//
//  return EXIT_SUCCESS;
//}

