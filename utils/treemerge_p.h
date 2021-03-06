/**
 * @file treemerge_p.h
 * The header file for the implementation part of 'treemerge', which compares
 * two tree sets with implied relationship that treeset1 gave raise to
 * treeset2 with extra mutations, and figures out which pairs are logically
 * correct. The purpose for using a separate implementation source file is
 * to decouple the logic from the command-line interface so that automated
 * test cases can be constructed.
 *
 * @author Yi Qiao
 */

/*
The MIT License (MIT)

Copyright (c) 2013 Yi Qiao

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef TREEMERGE_P_H
#define TREEMERGE_P_H

#include "SomaticEvent.h"
#include "Subclone.h"

/**
 * The boundary resolution when comparing SomaticEvents
 */
#define BOUNDRY_RESOLUTION 20000000L

using namespace SubcloneSeeker;

/**
 * Generate a list of events from a given subclone node. 
 * In the subclone data structure, events of a parent is not duplicated in 
 * the children nodes. This function will, from a given subclone node, trace
 * back to the root of the tree, and returning all the events it encounters.
 *
 * @param node The subclone node to generate event list from.
 * @return A vector of all the events explicitly or implicitly contained by the given node.
 */
SomaticEventPtr_vec nodeEventsList(Subclone * node);

/**
 * Calculate the difference between two vectors of somatic events
 *
 * @param master The somatic event vector that contains the wanted events
 * @param unwanted The somatic event vector that contains the unwanted events
 * @return A somatic event vector that contains all the events in 'master' that are not found in 'unwanted'
 */
SomaticEventPtr_vec SomaticEventDifference(
		const SomaticEventPtr_vec& master, 
		const SomaticEventPtr_vec& unwanted);

/**
 * Check if a somatic event vector contains all the events found in another vector.
 *
 * @param v_container The container vector
 * @param v_containee The containee vector
 * @return True if the container vector contains all the events found in the containee vector, false otherwise
 */
bool eventSetContains(
		const SomaticEventPtr_vec& v_container, 
		const SomaticEventPtr_vec& v_containee);

/**
 * Compare SomaticEvent vectors by size.
 *
 * @param v1 The first SomaticEventPtr vector
 * @param v2 The second SomaticEventPtr vector
 * @return True if v1 has less events than v2, false otherwise
 */
bool resultSetComparator(const SomaticEventPtr_vec& v1, const SomaticEventPtr_vec &v2);

/**
 * Check if a node with certain somatic events can be placed on a subtree of a different subclonal structure.
 *
 * @param pnode The root of a subtree of the subclonal structure, to which the new node is being placed on.
 * @param somaticEvents The somatic events found in the new node, containing all its parents' ones.
 * @param placeableOnSubtree An output boolean variable indicating whether the placement is successful or not.
 * @param cp The number of children nodes that are able to contain the floating node. Used for debugging purpose.
 * @return A vector containing events not found on the subtree to the point the node is placed.
 */
SomaticEventPtr_vec checkPlacement(
		Subclone *pnode, 
		SomaticEventPtr_vec somaticEvents, 
		bool * placeableOnSubtree,
		int * cp = NULL);

/**
 * Check if two subclonal trees are compatible.
 *
 * @param p The first subclone tree
 * @param q The second subclone tree
 * @return True if the two trees are compatible, false otherwise
 */
bool TreeMerge(Subclone *p, Subclone *q);
#endif
