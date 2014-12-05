<?php

/**
 * @file
 * The default format for adresses.
 */

$plugin = array(
  'title' => t('Address form (country-specific)'),
  'format callback' => 'addressfield_format_address_generate',
  'type' => 'address',
  'weight' => -100,
);

/**
 * Format callback.
 *
 * @see CALLBACK_addressfield_format_callback()
 */
function addressfield_format_address_generate(&$format, $address, $context = array()) {
  // We start with a reasonable default: a simple block format suitable
  // for international shipping. We extend it with country-specific heuristics
  // below.

  // The street block.
  $format['street_block'] = array(
    '#type' => 'addressfield_container',
    '#attributes' => array(
      'class' => array('street-block'),
    ),
    '#weight' => 0,
  );
  $format['street_block']['thoroughfare'] = array(
    '#title' => t('Address 1'),
    '#tag' => 'div',
    '#attributes' => array(
      'class' => array('thoroughfare'),
      'x-autocompletetype' => 'address-line1',
      'autocomplete' => 'address-line1',
    ),
    '#size' => 30,
    // The #required will be automatically set to FALSE when processing.
    '#required' => TRUE,
  );
  $format['street_block']['premise'] = array(
    '#title' => t('Address 2'),
    '#tag' => 'div',
    '#attributes' => array(
      'class' => array('premise'),
      'x-autocompletetype' => 'address-line2',
      'autocomplete' => 'address-line2',
    ),
    '#size' => 30,
  );
  $format['locality_block'] = array(
    '#type' => 'addressfield_container',
    '#attributes' => array(
      'class' => array('addressfield-container-inline', 'locality-block', 'country-' . $address['country']),
    ),
    '#weight' => 50,
  );
  $format['locality_block']['#attached']['css'][] = drupal_get_path('module', 'addressfield') . '/addressfield.css';
  $format['locality_block']['postal_code'] = array(
    '#title' => t('Postal code'),
    '#size' => 10,
    '#required' => FALSE,
    '#attributes' => array(
      'class' => array('postal-code'),
      'x-autocompletetype' => 'postal-code',
      'autocomplete' => 'postal-code',
    ),
  );
  $format['locality_block']['locality'] = array(
    '#title' => t('City'),
    '#size' => 30,
    '#required' => TRUE,
    '#prefix' => ' ',
    '#attributes' => array(
      'class' => array('locality'),
      'x-autocompletetype' => 'locality',
      'autocomplete' => 'locality',
    ),
  );
  $format['country'] = array(
    '#title' => t('Country'),
    '#options' => _addressfield_country_options_list(),
    '#render_option_value' => TRUE,
    '#required' => TRUE,
    '#attributes' => array(
      'class' => array('country'),
      'x-autocompletetype' => 'country',
      'autocomplete' => 'country',
    ),
    '#weight' => 100,
  );

  // These countries do not use postal codes.
  $countries_no_postal_code = array(
    'AO', 'AE', 'AF', 'AG', 'AI', 'BF', 'BI', 'BJ', 'BO', 'BS', 'BW', 'BZ', 'CD',
    'CF', 'CG', 'CI', 'CM', 'CO', 'DJ', 'DM', 'ER', 'FJ', 'GD', 'GH', 'GM', 'GQ',
    'GY', 'HK', 'IE', 'JM', 'KI', 'KM', 'KN', 'KP', 'LC', 'LY', 'ML', 'MO', 'MR',
    'MW', 'NR', 'PA', 'QA', 'RW', 'SB', 'SC', 'SL', 'SR', 'ST', 'TD', 'TG', 'TL',
    'TO', 'TV', 'TZ', 'UG', 'VU', 'ZW',
  );
  if (in_array($address['country'], $countries_no_postal_code)) {
    unset($format['locality_block']['postal_code']);

    // Remove the prefix from the first widget of the block.
    $element_children = element_children($format['locality_block']);
    $first_child = reset($element_children);
    unset($format['locality_block'][$first_child]['#prefix']);
  }

  // These countries have required postal codes.
  $countries_required_postal_code = array(
    'AS', 'AT', 'AU', 'AX', 'BE', 'BL', 'BR', 'CA', 'CH', 'CN', 'DE', 'DK', 'ES',
    'FI', 'FK', 'FM', 'FR', 'GB', 'GF', 'GG', 'GL', 'GP', 'GR', 'GS', 'GU', 'IM',
    'IN', 'IO', 'IT', 'JE', 'JP', 'KR', 'LI', 'LU', 'MF', 'MH', 'MP', 'MQ', 'MX',
    'MY', 'NC', 'NL', 'NO', 'NZ', 'PF', 'PL', 'PM', 'PN', 'PR', 'PT', 'PW', 'RE',
    'RU', 'SE', 'SG', 'SH', 'SJ', 'SM', 'TC', 'TR', 'TW', 'UA', 'US', 'VI', 'WF',
    'YT', 'ZA',
  );
  if (in_array($address['country'], $countries_required_postal_code)) {
    $format['locality_block']['postal_code']['#required'] = TRUE;
  }

  // These countries use the administrative area in postal addresses.
  $countries_administrative_area = array(
    'AE', 'AM', 'AR', 'AU', 'BR', 'BS', 'BY', 'CA', 'CL', 'CN', 'CO', 'CV', 'EG',
    'ES', 'FM', 'GB', 'HK', 'HN', 'ID', 'IE', 'IN', 'IT', 'IQ', 'IR', 'JP', 'KI',
    'KN', 'KR',  'KY', 'KZ', 'JM', 'MN', 'MX', 'MY', 'MZ', 'NG', 'NI', 'NR', 'NZ',
    'PA', 'PE', 'PF', 'PG', 'PH', 'PW', 'RU', 'SC', 'SO', 'SR', 'SV', 'TH', 'TR',
    'TV', 'TW', 'UA', 'US', 'UY', 'UZ', 'VE', 'VI', 'VN',
  );
  if (in_array($address['country'], $countries_administrative_area)) {
    $format['locality_block']['administrative_area'] = array(
      '#title' => t('State', array(), array('context' => 'Territory of a country')),
      '#size' => 10,
      '#required' => TRUE,
      '#prefix' => ' ',
      '#attributes' => array(
        'class' => array('state'),
        'x-autocompletetype' => 'region',
        'autocomplete' => 'region',
      ),
    );
  }

  // A few countries have a well-known list of administrative divisions.
  if ($address['country'] == 'US') {
    $format['locality_block']['administrative_area']['#options'] = array(
      ''   => t('--'),
      'AL' => t('Alabama'),
      'AK' => t('Alaska'),
      'AZ' => t('Arizona'),
      'AR' => t('Arkansas'),
      'CA' => t('California'),
      'CO' => t('Colorado'),
      'CT' => t('Connecticut'),
      'DE' => t('Delaware'),
      'DC' => t('District of Columbia'),
      'FL' => t('Florida'),
      'GA' => t('Georgia'),
      'HI' => t('Hawaii'),
      'ID' => t('Idaho'),
      'IL' => t('Illinois'),
      'IN' => t('Indiana'),
      'IA' => t('Iowa'),
      'KS' => t('Kansas'),
      'KY' => t('Kentucky'),
      'LA' => t('Louisiana'),
      'ME' => t('Maine'),
      'MD' => t('Maryland'),
      'MA' => t('Massachusetts'),
      'MI' => t('Michigan'),
      'MN' => t('Minnesota'),
      'MS' => t('Mississippi'),
      'MO' => t('Missouri'),
      'MT' => t('Montana'),
      'NE' => t('Nebraska'),
      'NV' => t('Nevada'),
      'NH' => t('New Hampshire'),
      'NJ' => t('New Jersey'),
      'NM' => t('New Mexico'),
      'NY' => t('New York'),
      'NC' => t('North Carolina'),
      'ND' => t('North Dakota'),
      'OH' => t('Ohio'),
      'OK' => t('Oklahoma'),
      'OR' => t('Oregon'),
      'PA' => t('Pennsylvania'),
      'RI' => t('Rhode Island'),
      'SC' => t('South Carolina'),
      'SD' => t('South Dakota'),
      'TN' => t('Tennessee'),
      'TX' => t('Texas'),
      'UT' => t('Utah'),
      'VT' => t('Vermont'),
      'VA' => t('Virginia'),
      'WA' => t('Washington'),
      'WV' => t('West Virginia'),
      'WI' => t('Wisconsin'),
      'WY' => t('Wyoming'),
      ' ' => t('--'),
      'AA' => t('Armed Forces (Americas)'),
      'AE' => t('Armed Forces (Europe, Canada, Middle East, Africa)'),
      'AP' => t('Armed Forces (Pacific)'),
      'AS' => t('American Samoa'),
      'FM' => t('Federated States of Micronesia'),
      'GU' => t('Guam'),
      'MH' => t('Marshall Islands'),
      'MP' => t('Northern Mariana Islands'),
      'PW' => t('Palau'),
      'PR' => t('Puerto Rico'),
      'VI' => t('Virgin Islands'),
    );
    $format['locality_block']['postal_code']['#title'] = t('ZIP Code');

    if ($context['mode'] == 'render') {
      $format['locality_block']['locality']['#suffix'] = ',';
    }
  }
  else if ($address['country'] == 'IT') {
    $format['locality_block']['administrative_area']['#options'] = array(
      '' => t('--'),
      'AG' => t('Agrigento'),
      'AL' => t('Alessandria'),
      'AN' => t('Ancona'),
      'AO' => t('Aosta'),
      'AP' => t('Ascoli Piceno'),
      'AQ' => t("L'Aquila"),
      'AR' => t('Arezzo'),
      'AT' => t('Asti'),
      'AV' => t('Avellino'),
      'BA' => t('Bari'),
      'BG' => t('Bergamo'),
      'BI' => t('Biella'),
      'BL' => t('Belluno'),
      'BN' => t('Benevento'),
      'BO' => t('Bologna'),
      'BR' => t('Brindisi'),
      'BS' => t('Brescia'),
      'BT' => t('Barletta-Andria-Trani'),
      'BZ' => t('Bolzano/Bozen'),
      'CA' => t('Cagliari'),
      'CB' => t('Campobasso'),
      'CE' => t('Caserta'),
      'CH' => t('Chieti'),
      'CI' => t('Carbonia-Iglesias'),
      'CL' => t('Caltanissetta'),
      'CN' => t('Cuneo'),
      'CO' => t('Como'),
      'CR' => t('Cremona'),
      'CS' => t('Cosenza'),
      'CT' => t('Catania'),
      'CZ' => t('Catanzaro'),
      'EN' => t('Enna'),
      'FC' => t('Forlì-Cesena'),
      'FE' => t('Ferrara'),
      'FG' => t('Foggia'),
      'FI' => t('Firenze'),
      'FM' => t('Fermo'),
      'FR' => t('Frosinone'),
      'GE' => t('Genova'),
      'GO' => t('Gorizia'),
      'GR' => t('Grosseto'),
      'IM' => t('Imperia'),
      'IS' => t('Isernia'),
      'KR' => t('Crotone'),
      'LC' => t('Lecco'),
      'LE' => t('Lecce'),
      'LI' => t('Livorno'),
      'LO' => t('Lodi'),
      'LT' => t('Latina'),
      'LU' => t('Lucca'),
      'MB' => t('Monza e Brianza'),
      'MC' => t('Macerata'),
      'ME' => t('Messina'),
      'MI' => t('Milano'),
      'MN' => t('Mantova'),
      'MO' => t('Modena'),
      'MS' => t('Massa-Carrara'),
      'MT' => t('Matera'),
      'NA' => t('Napoli'),
      'NO' => t('Novara'),
      'NU' => t('Nuoro'),
      'OG' => t('Ogliastra'),
      'OR' => t('Oristano'),
      'OT' => t('Olbia-Tempio'),
      'PA' => t('Palermo'),
      'PC' => t('Piacenza'),
      'PD' => t('Padova'),
      'PE' => t('Pescara'),
      'PG' => t('Perugia'),
      'PI' => t('Pisa'),
      'PN' => t('Pordenone'),
      'PO' => t('Prato'),
      'PR' => t('Parma'),
      'PT' => t('Pistoia'),
      'PU' => t('Pesaro e Urbino'),
      'PV' => t('Pavia'),
      'PZ' => t('Potenza'),
      'RA' => t('Ravenna'),
      'RC' => t('Reggio Calabria'),
      'RE' => t('Reggio Emilia'),
      'RG' => t('Ragusa'),
      'RI' => t('Rieti'),
      'RM' => t('Roma'),
      'RN' => t('Rimini'),
      'RO' => t('Rovigo'),
      'SA' => t('Salerno'),
      'SI' => t('Siena'),
      'SO' => t('Sondrio'),
      'SP' => t('La Spezia'),
      'SR' => t('Siracusa'),
      'SS' => t('Sassari'),
      'SV' => t('Savona'),
      'TA' => t('Taranto'),
      'TE' => t('Teramo'),
      'TN' => t('Trento'),
      'TO' => t('Torino'),
      'TP' => t('Trapani'),
      'TR' => t('Terni'),
      'TS' => t('Trieste'),
      'TV' => t('Treviso'),
      'UD' => t('Udine'),
      'VA' => t('Varese'),
      'VB' => t('Verbano-Cusio-Ossola'),
      'VC' => t('Vercelli'),
      'VE' => t('Venezia'),
      'VI' => t('Vicenza'),
      'VR' => t('Verona'),
      'VS' => t('Medio Campidano'),
      'VT' => t('Viterbo'),
      'VV' => t('Vibo Valentia'),
    );
    $format['locality_block']['administrative_area']['#title'] = t('Province');
  }
  else if ($address['country'] == 'ES') {
    $format['locality_block']['administrative_area']['#options'] = array(
      '' => t('--'),
      'C' => t("A Coruña"),
      'VI' => t('Alava'),
      'AB' => t('Albacete'),
      'A' => t('Alicante'),
      'AL' => t("Almería"),
      'O' => t('Asturias'),
      'AV' => t("Ávila"),
      'BA' => t('Badajoz'),
      'PM' => t('Baleares'),
      'B' => t('Barcelona'),
      'BU' => t('Burgos'),
      'CC' => t("Cáceres"),
      'CA' => t("Cádiz"),
      'S' => t('Cantabria'),
      'CS' => t("Castellón"),
      'CE' => t('Ceuta'),
      'CR' => t('Ciudad Real'),
      'CO' => t("Córdoba"),
      'CU' => t('Cuenca'),
      'GI' => t('Gerona'),
      'GR' => t('Granada'),
      'GU' => t('Guadalajara'),
      'SS' => t("Guipúzcoa"),
      'H' => t('Huelva'),
      'HU' => t('Huesca'),
      'J' => t("Jaén"),
      'LO' => t('La Rioja'),
      'GC' => t('Las Palmas'),
      'LE' => t("León"),
      'L' => t("Lérida"),
      'LU' => t('Lugo'),
      'M' => t('Madrid'),
      'MA' => t("Málaga"),
      'ML' => t('Melilla'),
      'MU' => t('Murcia'),
      'NA' => t('Navarra'),
      'OR' => t('Ourense'),
      'P' => t('Palencia'),
      'PO' => t('Pontevedra'),
      'SA' => t('Salamanca'),
      'TF' => t('Santa Cruz de Tenerife'),
      'SG' => t('Segovia'),
      'SE' => t('Sevilla'),
      'SO' => t('Soria'),
      'T' => t('Tarragona'),
      'TE' => t('Teruel'),
      'TO' => t('Toledo'),
      'V' => t('Valencia'),
      'VA' => t('Valladolid'),
      'BI' => t('Vizcaya'),
      'ZA' => t('Zamora'),
      'Z' => t('Zaragoza'),
    );
    $format['locality_block']['administrative_area']['#title'] = t('Province');
  }
  elseif ($address['country'] == 'BR') {
    $format['locality_block']['dependent_locality'] = array(
      '#title' => t('Neighborhood'),
      '#tag' => 'div',
      '#attributes' => array('class' => array('dependent-locality')),
      '#size' => 25,
      '#render_option_value' => FALSE,
    );
    $format['locality_block']['administrative_area']['#render_option_value'] = TRUE;
    $format['locality_block']['administrative_area']['#options'] = array(
      '' => t('--'),
      'AC' => t('Acre'),
      'AL' => t('Alagoas'),
      'AM' => t('Amazonas'),
      'AP' => t('Amapá'),
      'BA' => t('Bahia'),
      'CE' => t('Ceará'),
      'DF' => t('Distrito Federal'),
      'ES' => t('Espírito Santo'),
      'GO' => t('Goiás'),
      'MA' => t('Maranhão'),
      'MG' => t('Minas Gerais'),
      'MS' => t('Mato Grosso do Sul'),
      'MT' => t('Mato Grosso'),
      'PA' => t('Pará'),
      'PB' => t('Paraíba'),
      'PE' => t('Pernambuco'),
      'PI' => t('Piauí'),
      'PR' => t('Paraná'),
      'RJ' => t('Rio de Janeiro'),
      'RN' => t('Rio Grande do Norte'),
      'RO' => t('Rondônia'),
      'RR' => t('Roraima'),
      'RS' => t('Rio Grande do Sul'),
      'SC' => t('Santa Catarina'),
      'SE' => t('Sergipe'),
      'SP' => t('São Paulo'),
      'TO' => t('Tocantins'),
    );
    // Change some titles to make translation easier.
    $format['street_block']['#attributes'] = array(
      'class' => array('addressfield-container-inline'),
    );
    $format['street_block']['thoroughfare'] = array(
      '#title' => t('Thoroughfare'),
      '#tag'   => NULL,
      '#attributes' => array('class' => array('thoroughfare')),
      '#size' => 30,
      // The #required will be automatically set to FALSE when processing.
      '#required' => TRUE,
    );
    $format['street_block']['premise'] = array(
      '#title' => t('Complement'),
      '#tag' => NULL,
      '#attributes' => array('class' => array('premise')),
      '#size' => 20,
      '#prefix' => ', ',
    );
    $format['locality_block']['locality']['#suffix'] = ' - ';
    // Hide suffixes and prefixes while in form.
    if ($context['mode'] == 'form') {
      $format['street_block']['premise']['#prefix'] = NULL;
      $format['street_block']['premise']['#suffix'] = NULL;
      $format['locality_block']['locality']['#suffix'] = NULL;
    }
    // Render an extra field for 'Neighborhood'.
    $format['locality_block']['dependent_locality']['#render_option_value'] = TRUE;
    // Change some weights to conform local standards
    // Neighborhood.
    $format['locality_block']['dependent_locality']['#weight'] = 0;
    // City.
    $format['locality_block']['locality']['#weight'] = 5;
    // State.
    $format['locality_block']['administrative_area']['#weight'] = 10;
    // Postal Code.
    $format['locality_block']['postal_code']['#weight'] = 16;
    $format['locality_block']['postal_code']['#tag'] = 'div';
  }
  else if ($address['country'] == 'CA') {
    $format['locality_block']['administrative_area']['#options'] = array(
      '' => t('--'),
      'AB' => t('Alberta'),
      'BC' => t('British Columbia'),
      'MB' => t('Manitoba'),
      'NB' => t('New Brunswick'),
      'NL' => t('Newfoundland and Labrador'),
      'NT' => t('Northwest Territories'),
      'NS' => t('Nova Scotia'),
      'NU' => t('Nunavut'),
      'ON' => t('Ontario'),
      'PE' => t('Prince Edward Island'),
      'QC' => t('Quebec'),
      'SK' => t('Saskatchewan'),
      'YT' => t('Yukon Territory'),
    );
    $format['locality_block']['administrative_area']['#title'] = t('Province');

    if ($context['mode'] == 'render') {
      $format['locality_block']['locality']['#suffix'] = ',';
    }
  }
  else if ($address['country'] == 'AU') {
    $format['locality_block']['administrative_area']['#options'] = array(
      '' => t('--'),
      'ACT' => t('Australian Capital Territory'),
      'NSW' => t('New South Wales'),
      'NT' => t('Northern Territory'),
      'QLD' => t('Queensland'),
      'SA' => t('South Australia'),
      'TAS' => t('Tasmania'),
      'VIC' => t('Victoria'),
      'WA' => t('Western Australia'),
    );
  }
  else if ($address['country'] == 'NZ') {
    $format['locality_block']['locality']['#title'] = ('Town/City');
    $format['locality_block']['postal_code']['#title'] = t('Postcode');
    $format['locality_block']['administrative_area']['#render_option_value'] = TRUE;
    $format['locality_block']['administrative_area']['#title'] = t('Region');
    $format['locality_block']['administrative_area']['#required'] = FALSE;
    $format['locality_block']['administrative_area']['#options'] = array(
      ''   => t('--'),
      'AUK' => t('Auckland'),
      'BOP' => t('Bay of Plenty'),
      'CAN' => t('Canterbury'),
      'HKB' => t("Hawke's Bay"),
      'MWT' => t('Manawatu-Wanganui'),
      'NTL' => t('Northland'),
      'OTA' => t('Otago'),
      'STL' => t('Southland'),
      'TKI' => t('Taranaki'),
      'WKO' => t('Waikato'),
      'WGN' => t('Wellington'),
      'WTC' => t('West Coast'),
      'GIS' => t('Gisborne District'),
      'MBH' => t('Marlborough District'),
      'NSN' => t('Nelson'),
      'TAS' => t('Tasman District'),
      'CIT' => t('Chatham Islands Territory'),
    );
  }
  elseif ($address['country'] == 'MY') {
    $format['locality_block']['administrative_area']['#options'] = array(
      ''   => t('--'),
      '01' => t('Johor'),
      '02' => t('Kedah'),
      '03' => t('Kelantan'),
      '14' => t('Kuala Lumpur'),
      '15' => t('Labuan'),
      '04' => t('Melaka'),
      '05' => t('Negeri Sembilan'),
      '06' => t('Pahang'),
      '08' => t('Perak'),
      '09' => t('Perlis'),
      '07' => t('Pulau Pinang'),
      '16' => t('Putrajaya'),
      '12' => t('Sabah'),
      '13' => t('Sarawak'),
      '10' => t('Selangor'),
      '11' => t('Terengganu'),
    );
  }
  else if ($address['country'] == 'MX') {
    $format['locality_block']['administrative_area']['#options'] = array(
      '' => t('--'),
      'AGU' => t('Aguascalientes'),
      'BCN' => t('Baja California'),
      'BCS' => t('Baja California Sur'),
      'CAM' => t('Campeche'),
      'COA' => t('Coahuila'),
      'COL' => t('Colima'),
      'CHP' => t('Chiapas'),
      'CHH' => t('Chihuahua'),
      'DIF' => t('Distrito Federal'),
      'DUG' => t('Durango'),
      'MEX' => t('Estado de México'),
      'GUA' => t('Guanajuato'),
      'GRO' => t('Guerrero'),
      'HID' => t('Hidalgo'),
      'JAL' => t('Jalisco'),
      'MIC' => t('Michoacán'),
      'MOR' => t('Morelos'),
      'NAY' => t('Nayarit'),
      'NLE' => t('Nuevo León'),
      'OAX' => t('Oaxaca'),
      'PUE' => t('Puebla'),
      'QUE' => t('Queretaro'),
      'ROO' => t('Quintana Roo'),
      'SLP' => t('San Luis Potosí'),
      'SIN' => t('Sinaloa'),
      'SON' => t('Sonora'),
      'TAB' => t('Tabasco'),
      'TAM' => t('Tamaulipas'),
      'TLA' => t('Tlaxcala'),
      'VER' => t('Veracruz'),
      'YUC' => t('Yucatán'),
      'ZAC' => t('Zacatecas'),
    );
  }
  elseif ($address['country'] == 'IN') {
    $format['locality_block']['administrative_area']['#options'] = array(
      ''   => t('--'),
      'AN' => t('Andaman & Nicobar'),
      'AP' => t('Andhra Pradesh'),
      'AR' => t('Arunachal Pradesh'),
      'AS' => t('Assam'),
      'BR' => t('Bihar'),
      'CH' => t('Chandigarh'),
      'CT' => t('Chhattisgarh'),
      'DD' => t('Daman & Diu'),
      'DL' => t('Delhi'),
      'DN' => t('Dadra & Nagar Haveli'),
      'GA' => t('Goa'),
      'GJ' => t('Gujarat'),
      'HP' => t('Himachal Pradesh'),
      'HR' => t('Haryana'),
      'JH' => t('Jharkhand'),
      'JK' => t('Jammu & Kashmir'),
      'KA' => t('Karnataka'),
      'KL' => t('Kerala'),
      'LD' => t('Lakshadweep'),
      'MH' => t('Maharashtra'),
      'MN' => t('Manipur'),
      'ML' => t('Meghalaya'),
      'MP' => t('Madhya Pradesh'),
      'MZ' => t('Mizoram'),
      'NL' => t('Nagaland'),
      'OR' => t('Odisha'),
      'PB' => t('Punjab'),
      'PY' => t('Puducherry'),
      'RJ' => t('Rajasthan'),
      'SK' => t('Sikkim'),
      'TN' => t('Tamil Nadu'),
      'TR' => t('Tripura'),
      'UP' => t('Uttar Pradesh'),
      'UT' => t('Uttarakhand'),
      'WB' => t('West Bengal'),
    );
    $format['locality_block']['administrative_area']['#title'] = t('State');
  }
  elseif ($address['country'] == 'PE') {
    $format['locality_block']['administrative_area']['#options'] = array(
      ''   => t('--'),
      'AMA' => t('Amazonas'),
      'ANC' => t('Ancash'),
      'APU' => t('Apurimac'),
      'ARE' => t('Arequipa'),
      'AYA' => t('Ayacucho'),
      'CAJ' => t('Cajamarca'),
      'CAL' => t('Callao'),
      'CUS' => t('Cusco'),
      'HUV' => t('Huancavelica'),
      'HUC' => t('Huanuco'),
      'ICA' => t('Ica'),
      'JUN' => t('Junin'),
      'LAL' => t('La Libertad'),
      'LAM' => t('Lambayeque'),
      'LIM' => t('Lima'),
      'LOR' => t('Loreto'),
      'MDD' => t('Madre de Dios'),
      'MOQ' => t('Moquegua'),
      'PAS' => t('Pasco'),
      'PIU' => t('Piura'),
      'PUN' => t('Puno'),
      'SAM' => t('San Martin'),
      'TAC' => t('Tacna'),
      'TUM' => t('Tumbes'),
      'UCA' => t('Ucayali'),
    );
    $format['locality_block']['administrative_area']['#title'] = t('Department');
    $format['locality_block']['locality']['#title'] = t('District');
    $format['locality_block']['administrative_area']['#weight'] = 1;
    $format['locality_block']['locality']['#weight'] = 2;
  }

  // Those countries tend to put the postal code after the locality.
  static $countries_postal_code_after_locality = array('AU', 'BD', 'BF', 'BH', 'BM', 'BN', 'BT', 'CA', 'FM', 'GB', 'ID', 'IN', 'JM', 'JO', 'KH', 'LB', 'LS', 'LV', 'MM', 'MN', 'MV', 'NG', 'NP', 'NZ', 'PK', 'PR', 'PW', 'SA', 'SG', 'SO', 'TH', 'US', 'VI', 'VG', 'VN');
  if (in_array($address['country'], $countries_postal_code_after_locality)) {
    // Take the widget out of the array.
    $postal_code_widget = $format['locality_block']['postal_code'];
    $postal_code_widget['#prefix'] = ' ';
    unset($format['locality_block']['postal_code']);

    // Add it back.
    $format['locality_block']['postal_code'] = $postal_code_widget;

    // Remove the prefix from the first widget of the block.
    $element_children = element_children($format['locality_block']);
    $first_child = reset($element_children);
    unset($format['locality_block'][$first_child]['#prefix']);
  }

  // GB-specific tweaks
  if ($address['country'] == 'GB') {
    // Locality
    $format['locality_block']['locality'] = array_merge(
      $format['locality_block']['locality'],
      array(
        '#title' => t('Town/City'),
        '#weight' => 40,
        '#prefix' => '',
        '#tag' => 'div',
      )
    );

    // Administrative
    $format['locality_block']['administrative_area'] = array_merge(
      $format['locality_block']['administrative_area'],
      array(
        '#title' => t('County'),
        '#required' => FALSE,
        '#weight' => 50,
        '#size' => 30,
        '#prefix' => '',
        '#tag' => 'div',
      )
    );

    // Postal code
    $format['locality_block']['postal_code'] = array_merge(
      $format['locality_block']['postal_code'],
      array(
        '#title' => t('Postcode'),
        '#weight' => 60,
        '#prefix' => '',
        '#tag' => 'div',
      )
    );
  }

  // JP-specific tweaks
  if ($address['country'] == 'JP') {
    // Change the order.
    $format['locality_block']['#weight'] = 10;
    $format['locality_block']['#attributes']['class'][0] = 'addressfield-container';
    $format['locality_block']['postal_code']['#weight'] = 10;
    $format['locality_block']['postal_code']['#tag'] = 'div';
    $format['locality_block']['postal_code']['#size'] = 30;
    $format['locality_block']['administrative_area']['#title'] = t('Prefecture');
    $format['locality_block']['administrative_area']['#weight'] = 20;
    $format['locality_block']['administrative_area']['#size'] = 30;
    $format['locality_block']['administrative_area']['#render_option_value'] = TRUE;
    $format['locality_block']['administrative_area']['#options'] = array(
      ''   => t('--'),
      '01' => t('Hokkaido'),
      '02' => t('Aomori'),
      '03' => t('Iwate'),
      '04' => t('Miyagi'),
      '05' => t('Akita'),
      '06' => t('Yamagata'),
      '07' => t('Fukushima'),
      '08' => t('Ibaraki'),
      '09' => t('Tochigi'),
      '10' => t('Gunma'),
      '11' => t('Saitama'),
      '12' => t('Chiba'),
      '13' => t('Tokyo'),
      '14' => t('Kanagawa'),
      '15' => t('Niigata'),
      '16' => t('Toyama'),
      '17' => t('Ishikawa'),
      '18' => t('Fukui'),
      '19' => t('Yamanashi'),
      '20' => t('Nagano'),
      '21' => t('Gifu'),
      '22' => t('Shizuoka'),
      '23' => t('Aichi'),
      '24' => t('Mie'),
      '25' => t('Shiga'),
      '26' => t('Kyoto'),
      '27' => t('Osaka'),
      '28' => t('Hyogo'),
      '29' => t('Nara'),
      '30' => t('Wakayama'),
      '31' => t('Tottori'),
      '32' => t('Shimane'),
      '33' => t('Okayama'),
      '34' => t('Hiroshima'),
      '35' => t('Yamaguchi'),
      '36' => t('Tokushima'),
      '37' => t('Kagawa'),
      '38' => t('Ehime'),
      '39' => t('Kochi'),
      '40' => t('Fukuoka'),
      '41' => t('Saga'),
      '42' => t('Nagasaki'),
      '43' => t('Kumamoto'),
      '44' => t('Oita'),
      '45' => t('Miyazaki'),
      '46' => t('Kagoshima'),
      '47' => t('Okinawa'),
    );
    $format['locality_block']['locality']['#weight'] = 30;
    $format['street_block']['#weight'] = 20;
  }

  if ($context['mode'] == 'form') {
    // Provide a wrapper ID for AJAX replacement based on country selection.
    if (!isset($format['#wrapper_id'])) {
      $format['#wrapper_id'] = drupal_html_id('addressfield-wrapper');
      $format['#prefix'] = '<div id="' . $format['#wrapper_id'] . '">';
      $format['#suffix'] = '</div>';
    }

    // Form mode, move the country selector to the top of the form.
    $format['country']['#weight'] = -10;

    // Limit it to the countries supported by the widget.
    if (isset($context['field'])) {
      $format['country']['#options'] = _addressfield_country_options_list($context['field'], $context['instance']);
    }

    // AJAX enable it.
    $format['country']['#ajax'] = array(
      'callback' => 'addressfield_standard_widget_refresh',
      'wrapper' => $format['#wrapper_id'],
    );
    $format['country']['#element_validate'] = array('addressfield_standard_country_validate');
    // Don't validate any element when the country is changed.
    $format['country']['#limit_validation_errors'] = array();

    if (isset($context['delta']) && $context['delta'] > 0) {
      // On subsequent elements of a field, we make the country field non
      // required and add a ' - None - ' option to it, so as to allow the
      // user to remove the address by clearing the country field.
      $format['country']['#required'] = FALSE;
      $format['country']['#empty_value'] = '';
    }
  }
}
