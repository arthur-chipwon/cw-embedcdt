/*******************************************************************************
 * Copyright (c) 2017 Liviu Ionescu.
 *
 * This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License 2.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *     Liviu Ionescu - initial version
 *     Liviu Ionescu - UI part extraction.
 *******************************************************************************/

package org.eclipse.embedcdt.internal.ui;

import org.eclipse.osgi.util.NLS;

public class Messages extends NLS {

	// ------------------------------------------------------------------------

	private static final String MESSAGES = "org.eclipse.embedcdt.internal.ui.messages"; //$NON-NLS-1$

	public static String McuPropertiesPage_description;
	public static String McuPreferencesPage_description;
	public static String XpackBrowseDialog_Label_text;

	// ------------------------------------------------------------------------

	static {
		// Initialize resource bundle
		NLS.initializeMessages(MESSAGES, Messages.class);
	}

	private Messages() {
	}

}
