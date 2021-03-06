<?php 

// affili.net API Calls

// Username and passwords
if (isset($_COOKIE['Drupal_visitor_affilinet_username_active'])) {
	define ("USERNAME", $_COOKIE['Drupal_visitor_affilinet_username_active'] );
} else {
	define ("USERNAME", variable_get('affilinet_username_active')); // the publisher ID für DE
}

define ("PASSWORD", variable_get('affilinet_password')); // the publisher web services password
define ("PRODPASSWORD", variable_get('affilinet_prodpassword')); // the product web services password

// API
define ("WSDL_LOGON", "https://api.affili.net/V2.0/Logon.svc?wsdl");
define ("WSDL_SERVICE",  "https://api.affili.net/V2.0/PublisherProgram.svc?wsdl");
define ("WSDL_STATS",  "https://api.affili.net/V2.0/PublisherProgram.svc?wsdl");
define ("WSDL_PROD",  "https://api.affili.net/V2.0/ProductServices.svc?wsdl");
define ("WSDL_CREAT",  "https://api.affili.net/V2.0/PublisherCreative.svc?wsdl");
define ("WSDL_PROGRAM",  "https://api.affili.net/V2.0/PublisherProgram.svc?wsdl");
define ("WSDL_WS",  "https://api.affili.net/V2.0/PublisherProgram.svc?wsdl");
define ("WSDL_REQ",  "https://api.affili.net/V2.0/PublisherProgram.svc?wsdl");
define ("WSDL_DATA",  "http://product-api.affili.net/V3/WSDLFactory/Product_ProductData.wsdl");
define ("WSDL_DATA_LOGON",  "http://product-api.affili.net/Authentication/Logon.svc?wsdl");
define("WSDL_ACCOUNT", "https://api.affili.net/V2.0/AccountService.svc?wsdl");
define("WSDL_INBOX", "https://api.affili.net/V2.0/PublisherInbox.svc?wsdl");
define("WSDL_PUBSTATS", "https://api.affili.net/V2.0/PublisherStatistics.svc?wsdl");

function _affilinet_GetMyPrograms( $query) {
	$SOAP_LOGON = new SoapClient(WSDL_LOGON);
	$Token      = $SOAP_LOGON->Logon(array(
				 'Username'  => USERNAME,
				 'Password'  => PASSWORD,
				 'WebServiceType' => 'Publisher'
				 ));

	$params = array('Query' => $query);

	$SOAP_REQUEST = new SoapClient(WSDL_STATS);
	$req = $SOAP_REQUEST->GetMyPrograms(array(
				'CredentialToken' => $Token,
				'GetProgramsRequestMessage' => $params
				));

	return $req;
}


function _affilinet_GetProgramRates($ProgramId) {
	$SOAP_LOGON = new SoapClient(WSDL_LOGON);
	$Token      = $SOAP_LOGON->Logon(array(
				 'Username'  => USERNAME,
				 'Password'  => PASSWORD,
				 'WebServiceType' => 'Publisher'
				 ));

	$SOAP_REQUEST = new SoapClient(WSDL_SERVICE); $req = $SOAP_REQUEST->GetProgramRates(array(
				'CredentialToken' => $Token,
				'PublisherId' => USERNAME,
				'ProgramId' => $ProgramId
			));
	 
	return $req;
}

function _affilinet_GetCreativeCategories($ProgramId) {
		
	$SOAP_LOGON = new SoapClient(WSDL_LOGON);
	$Token      = $SOAP_LOGON->Logon(array(
	             'Username'  => USERNAME,
	             'Password'  => PASSWORD,
	             'WebServiceType' => 'Publisher'
	             ));
	
	$SOAP_REQUEST = new SoapClient(WSDL_CREAT);
	$req = $SOAP_REQUEST->GetCreativeCategories(array(
	            'CredentialToken' => $Token,
				'ProgramId' => $ProgramId
	            ));
	return $req;
}

function _affilinet_SearchCreatives($ProgramIds) { //muss ein array sein
	$SOAP_LOGON = new SoapClient(WSDL_LOGON);
	$Token      = $SOAP_LOGON->Logon(array(
				 'Username'  => USERNAME,
				 'Password'  => PASSWORD,
				 'WebServiceType' => 'Publisher'
				 ));

	$DisplaySettings = array (
				'PageSize' => 100,
				'CurrentPage' => 1
				);
	$SearchCreativesQuery = array(
				'ProgramIds' => $ProgramIds
				);
				
	$SOAP_REQUEST = new SoapClient(WSDL_CREAT);
	$req = $SOAP_REQUEST->SearchCreatives(array(
				'CredentialToken' => $Token,
				'DisplaySettings' => $DisplaySettings,
				'SearchCreativesQuery' => $SearchCreativesQuery
				));
	return $req;
}

function _affilinet_GetPrograms($ProgramIds, $SearchString, $ProgramCategoryIds, $currentPage = 1, $pageSize = 100) {
	$SOAP_LOGON = new SoapClient(WSDL_LOGON);
	$Token      = $SOAP_LOGON->Logon(array(
	             'Username'  => USERNAME,
	             'Password'  => PASSWORD,
	             'WebServiceType' => 'Publisher'
	             ));
	 
	$DisplaySettings = array (
				'PageSize' => $pageSize,
				'CurrentPage' => $currentPage,
				'SortByEnum' => 'ProgramTitle'
				);
				
	$GetProgramsQuery = array();
	$GetProgramsQuery['PartnershipStatus'] = array('Active');
	if ($ProgramIds != '') {$GetProgramsQuery['ProgramIds'] = $ProgramIds;}
	if ($SearchString != '') {$GetProgramsQuery['SearchString'] = $SearchString;}
	if ($ProgramCategoryIds != '') {$GetProgramsQuery['ProgramCategoryIds'] = $ProgramCategoryIds;}
	
	$SOAP_REQUEST = new SoapClient(WSDL_PROGRAM);
	$req = $SOAP_REQUEST->GetPrograms(array(
	            'CredentialToken' => $Token,
				'DisplaySettings' => $DisplaySettings,
	            'GetProgramsQuery' => $GetProgramsQuery
	            ));
	return $req;
}

function _affilinet_GetShopList($query, $currentPage=1, $pageSize=100) {
	$SOAP_PROD_LOGON = new SoapClient(WSDL_LOGON);
	$token = $SOAP_PROD_LOGON->Logon(array(
				  'Username'  => USERNAME,
				  'Password'  => PRODPASSWORD,
				  'WebServiceType' => 'Product'
				  ));
    // Set page setting parameters
    $pageSettings = array(
        'CurrentPage' => 1,
        'PageSize' => 5000
    );
 
    // Set parameters
    $params = array(
        'CredentialToken' => $token,
        'LogoScale' => 'Logo120',
        'PageSettings' => $pageSettings,
        'Query' => $query
    );
 
    // Send request to Publisher Program Service
    $soapRequest = new SoapClient(WSDL_PROD);
    $response = $soapRequest->GetShopList($params);
	
	return $response;
}

function _affilinet_GetCategoryList($shopId) {
    // Send a request to the Logon Service to get an authentication token
    $soapLogon = new SoapClient(WSDL_LOGON);
    $token = $soapLogon->Logon(array(
        'Username' => USERNAME,
        'Password' => PRODPASSWORD,
        'WebServiceType' => 'Product'
    ));
 
    // Set page setting parameters
    $pageSettings = array(
        'CurrentPage' => 1,
        'PageSize' => 5000
    );
 
    // Set parameters
    $params = array(
    	'GetCategoryListRequestMessage' => 0,
        'CredentialToken' => $token,
        'PublisherId' => USERNAME, // the Id of the requesting publisher (mandatory)
        'PageSettings' => $pageSettings,
        'ShopId' => $shopId, // use "GetShopList" to get Shop Ids (mandatory)
    );
 
    // Send request to Publisher Program Service
    $soapRequest = new SoapClient(WSDL_PROD);
    $response = $soapRequest->GetCategoryList($params);
	
	return $response;
}

function _affilinet_GetProgramCategories() {
	$SOAP_LOGON = new SoapClient(WSDL_LOGON);
	$Token      = $SOAP_LOGON->Logon(array(
				 'Username'  => USERNAME,
				 'Password'  => PASSWORD,
				 'WebServiceType' => 'Publisher'
				 ));

	$SOAP_REQUEST = new SoapClient(WSDL_WS);
	$req = $SOAP_REQUEST->GetProgramCategories($Token);

	return $req;
}

function _affilinet_GetProgramListByCategory($CategoryId) {
	$SOAP_LOGON = new SoapClient(WSDL_LOGON);
	$Token      = $SOAP_LOGON->Logon(array(
				 'Username'  => USERNAME,
				 'Password'  => PASSWORD,
				 'WebServiceType' => 'Publisher'
				 ));
	$params = array(
			  'CategoryId' => $CategoryId
			  );

	$SOAP_REQUEST = new SoapClient(WSDL_REQ);
	$req = $SOAP_REQUEST->GetProgramListByCategory(array(
				'CredentialToken' => $Token,
				'GetProgramListByCategoryRequestMessage' => $params
				));
				
	return $req;
}


function _affilinet_GetPropertyList($ShopId) {
	$SOAP_PROD_LOGON = new SoapClient(WSDL_DATA_LOGON);
	$ProdToken      = $SOAP_PROD_LOGON->Logon(array(
				  'Username'  => USERNAME,
				  'Password'  => PRODPASSWORD,
				  'WebServiceType' => 'Product'
				  ));
	 
	$SOAP_PROD_REQUEST = new SoapClient(WSDL_DATA);
	$req = $SOAP_PROD_REQUEST->GetPropertyList(array(
				 'PublisherID' => USERNAME,
				 'CredentialToken' => $ProdToken,
				  'ShopId' => $ShopId
				 ));

	return $req;
}


function removeUTF8BOM($json_output) {
    if (substr($json_output, 0, 3) === "\xEF\xBB\xBF") {
        return substr($json_output, 3);
    }
    return $json_output;
}

function _affilinet_SearchProductsJSON($query) {

	$url = "http://product-api.affili.net/V3/productservice.svc/JSON/SearchProducts?publisherId=736510&Password=n7u8ueQIATdC1RXcyCbn&SortBy=Price&SortOrder=descending&PageSize=25&WithImageOnly=true&ImageScales=Image120&Query=" . $_GET['q'];
	$json = file_get_contents($url);
	$json = removeUTF8BOM($json);

	$json_output = json_decode($json, TRUE);

	return $json_output;
}

function _affilinet_SearchProducts($query, $sortBy, $sortOrder, $minPrice=0.0, $maxPrice=0.0, $pageSize=10, $currentPage=1,$categories,$filter) {

	$SOAP_PROD_LOGON = new SoapClient(WSDL_DATA_LOGON);
	$ProdToken = $SOAP_PROD_LOGON->Logon(array(
				  'Username'  => USERNAME,
				  'Password'  => PRODPASSWORD,
				  'WebServiceType' => 'Product'
				  ));
	 
	$SOAP_PROD_REQUEST = new SoapClient(WSDL_DATA);
	$req = $SOAP_PROD_REQUEST->SearchProducts(array(
					'PublisherId' => USERNAME,
					'CredentialToken' => $ProdToken,
					'ShopIds' => array('0'),
					'Query'  => $query,
					'WithImageOnly' => true,
					'ImageScales' => array('Image120'),
					'LogoScales' => array('Logo120'),
					'PageSettings' => array('CurrentPage' => $currentPage,'PageSize' => $pageSize),
					'MinimumPrice' => $minPrice,
					'MaximumPrice' => $maxPrice,
					'SortBy' => $sortBy,
					'SortOrder' => $sortOrder,
					'CategoryIds' => $categories,
					'UseAffilinetCategories' => TRUE,
					'FilterQueries' => $filter
				 ));
	return $req;
}

function _affilinet_GetPublisherSummary() {
	// Send a request to the Logon Service to get an authentication token
	$soapLogon = new SoapClient(WSDL_LOGON);
	$token  = $soapLogon->Logon(array(
	    'Username' => USERNAME,
	    'Password' => PASSWORD,
	    'WebServiceType' => 'Publisher'
	));
	 
	// Send a request to the Account Service 
	$soapRequest = new SoapClient(WSDL_ACCOUNT); 
	$response = $soapRequest->GetPublisherSummary($token);
	
	affilinet_print_pre($response);
	
	return $response;	
}

function _affilinet_GetProgramInfoMessages() {
	 
	// Send a request to the Logon Service to get an authentication token
	$soapLogon = new SoapClient(WSDL_LOGON);
	$token = $soapLogon->Logon(array(
	    'Username'  => USERNAME,
	    'Password'  => PASSWORD,
	    'WebServiceType' => 'Publisher'
	));
	 
	// Set parameters
	$params = array(
	    'TimeSpan' => 'Last7days', // timespan of the messages 
	    'Query' => '', // the search query
	    'MessageStatus' => 'All' // status of the messages
	);
	 
	// Send a request to the Publisher Inbox Service  
	$soapRequest = new SoapClient(WSDL_INBOX);
	$response = $soapRequest->GetProgramInfoMessages(array(
	    'CredentialToken' => $token,
	    'GetProgramInfoMessagesRequestMessage' => $params
	));	

	return $response;
}

function _affilinet_GetProgramStatusMessages() {
	// Send a request to the Logon Service to get an authentication token
	$soapLogon = new SoapClient(WSDL_LOGON);
	$token = $soapLogon->Logon(array(
	    'Username' => USERNAME,
	    'Password' => PASSWORD,
	    'WebServiceType' => 'Publisher'
	));
	 
	// Set parameters
	$params = array(
	    'TimeSpan' => 'Last7days',
	    'Query' => '',
	    'MessagePartnershipStatus' => 'AllAcceptedPartnerships'
	);
	 
	// Send request to Publisher Inbox Service
	$soapRequest = new SoapClient(WSDL_INBOX);
	$response = $soapRequest->GetProgramStatusMessages(array(
	    'CredentialToken' => $token,
	    'GetProgramStatusMessagesRequestMessage' => $params
	));	
	
	return $response;
}

function _affilinet_SearchVoucherCodes($currentPage=1, $programId = '', $query = ''){
	// Send a request to the Logon Service to get an authentication token
	$soapLogon = new SoapClient(WSDL_LOGON);
	$token = $soapLogon->Logon(array(
	    'Username'  => USERNAME,
	    'Password'  => PASSWORD,
	    'WebServiceType' => 'Publisher'
	));    
	 
	// Set DisplaySettings parameters
	$displaySettings = array(
	    'CurrentPage' => $currentPage,
	    'PageSize' => 25,
	    'SortBy' => 'LastChangeDate',
	    'SortOrder' => 'Descending'
	);
	 
	// Set SearchVoucherCodesRequestMessage parameters
	$params = array(
		'ProgramId' => $programId,
		'Query' => $query,
	    'StartDate' => strtotime("now"),
	    'EndDate' => strtotime("now"),
	    'ProgramPartnershipStatus' => 'Accepted'
	);
	 
	// Send a request to the Publisher Inbox Service  
	$soapRequest = new SoapClient(WSDL_INBOX);
	$response = $soapRequest->SearchVoucherCodes(array(
	    'CredentialToken' => $token,
	    'DisplaySettings' => $displaySettings,
	    'SearchVoucherCodesRequestMessage' => $params
	));
	return $response;
}

function _affilinet_GetSubIdStatistics($subId) {
	// Send a request to the Logon Service to get an authentication token
	$soapLogon = new SoapClient(WSDL_LOGON);
	$token = $soapLogon->Logon(array(
	    'Username' => USERNAME,
	    'Password' => PASSWORD,
	    'WebServiceType' => 'Publisher'
	));

	// Set parameters
	$startDate = strtotime("-1 year");
	$endDate = strtotime("today");
	$programIds = array('0');
	$params = array(
	    'StartDate' => $startDate,
	    'EndDate' => $endDate,
	    'ProgramIds' => $programIds,
	    'ProgramTypes' => 'All',
	    'SubId' => $subId,
	    'MaximumRecords' => '100',
	    'TransactionStatus' => 'All',
	    'ValuationType' => 'DateOfRegistration'
	);
	 
	// Send a request to the Publisher Statistics Service
	$soapRequest = new SoapClient(WSDL_PUBSTATS);
	$response = $soapRequest->GetSubIdStatistics(array(
	    'CredentialToken' => $token,
	    'GetSubIdStatisticsRequestMessage' => $params
	));	
	
	return $response;
}

function _affilinet_GetClicksBySubIdPerDay($programId) {
	// Send a request to the Logon Service to get an authentication token
	$soapLogon = new SoapClient(WSDL_LOGON);
	$token = $soapLogon->Logon(array(
	    'Username' => USERNAME,
	    'Password' => PASSWORD,
	    'WebServiceType' => 'Publisher'
	));
	 
	// Set parameters
	$startDate = strtotime("-2 months");
	$endDate = strtotime("today");
	$params = array(
	    'StartDate' => $startDate,
	    'EndDate' => $endDate,
	    'UseGrossValues' => true,
	    'ProgramId' => $programId
	);
	 
	// Send request to Publisher Statistics Service
	$soapRequest = new SoapClient(WSDL_PUBSTATS);
	$response = $soapRequest->GetClicksBySubIdPerDay(array(
	    'CredentialToken' => $token,
	    'GetClicksBySubIdPerDayRequestMessage' => $params
	));	
	return $response;
		
}

function _affilinet_GetProgramStatistics($programIds) {
	// Send a request to the Logon Service to get an authentication token
	$soapLogon = new SoapClient(WSDL_LOGON);
	$token = $soapLogon->Logon(array(
	    'Username'  => USERNAME,
	    'Password'  => PASSWORD,
	    'WebServiceType' => 'Publisher'
	));
	 
	// Set params
	$startDate = strtotime("-1 month");
	$endDate = strtotime("today");

	$params = array(
	    'StartDate' => $startDate,
	    'EndDate' => $endDate,
	    'ProgramStatus' => 'Active',
	    'ProgramIds' => $programIds,
	    'SubId' => '',
	    'ProgramTypes' => 'All',
	    'ValuationType' => 'DateOfRegistration'
	);
	 
	// Send request to Publisher Statistics Service
	$soapRequest = new SoapClient(WSDL_PUBSTATS);
	$response = $soapRequest->GetProgramStatistics(array(
	      'CredentialToken' => $token,
	      'GetProgramStatisticsRequestMessage' => $params
	));	
	
	return $response;
}

function _affilinet_GetDailyStatistics($programId,$subId, $days) {
	// Send a request to the Logon Service to get an authentication token
	$soapLogon = new SoapClient(WSDL_LOGON);
	$token = $soapLogon->Logon(array(
	    'Username' => USERNAME,
	    'Password' => PASSWORD,
	    'WebServiceType' => 'Publisher'
	));  
	 
	// Set parameters
	$startDate = strtotime("-".$days." days");
	$endDate = strtotime("today");
	$params = array(
	    'StartDate' => $startDate,
	    'EndDate' => $endDate,
	    'ProgramId' => $programId,
	    'SubId' => $subId,
	    'ProgramTypes' => 'All',
	    'ValuationType' => 'DateOfRegistration'
	);
	 
	// Send request to Publisher Statistics Service
	$soapRequest = new SoapClient(WSDL_PUBSTATS);
	$response = $soapRequest->GetDailyStatistics(array(
	    'CredentialToken' => $token,
	    'GetDailyStatisticsRequestMessage' => $params
	));	
	
	return $response;
}

function _affilinet_GetPublisherClicksSummary($programId,$subId){
	// Send a request to the Logon Service to get an authentication token
	$soapLogon = new SoapClient(WSDL_LOGON);
	$token = $soapLogon->Logon(array(
	    'Username' => USERNAME,
	    'Password' => PASSWORD,
	    'WebServiceType' => 'Publisher'
	));
	 
	// Set parameters          
	$startDate = strtotime("-2 weeks");
	$endDate = strtotime("today");
	$params = array(
	    'ProgramId' => $programId,
	    'StartDate' => $startDate,
	    'EndDate' => $endDate,
	    'SubId' => $subId
	);
	 
	// Send request to Publisher Statistics Service
	$soapRequest = new SoapClient(WSDL_PUBSTATS);
	$response = $soapRequest->GetPublisherClicksSummary(array(
	    'CredentialToken' => $token,
	    'GetPublisherClicksSummaryRequestMessage' => $params
	));
	return $response;	
}

function _affilinet_GetPublisherStatisticsPerClick($programId){
	// Send a request to the Logon Service to get an authentication token
	$soapLogon = new SoapClient(WSDL_LOGON);
	$token = $soapLogon->Logon(array(
	    'Username' => USERNAME,
	    'Password' => PASSWORD,
	    'WebServiceType' => 'Publisher'
	));
	 
	// Set parameters
	$startDate = strtotime("-1 day");
	$endDate = strtotime("today");
	$params = array(
	    'ProgramId' => $programId,
	    'StartDate' => $startDate,
	    'EndDate' => $endDate,
	    'SubId' => '',
	    'SortFilter' => 'Time'
	);    
	 
	// Send request to Publisher Statistics Service
	$soapRequest = new SoapClient(WSDL_PUBSTATS);
	$response = $soapRequest->GetPublisherStatisticsPerClick(array(
	    'CredentialToken' => $token,
	    'GetPublisherStatisticsPerClickRequestMessage' => $params
	));

	return $response;	
}

function _affilinet_GetPayments() {
	// Send a request to the Logon Service to get an authentication token
	$soapLogon = new SoapClient(WSDL_LOGON);
	$token = $soapLogon->Logon(array(
	    'Username' => USERNAME,
	    'Password' => PASSWORD,
	    'WebServiceType' => 'Publisher'
	));                        
	 
	// Set parameters
	$startDate = strtotime("-1 year");
	$endDate = strtotime("today");
	$publisherId = USERNAME; // the publisher ID you want to retrieve payments for (mandatory)
	 
	// Send a request to the Account Service 
	$soapRequest = new SoapClient(WSDL_ACCOUNT); 
	$response = $soapRequest->GetPayments(array(
	    'CredentialToken' => $token,                
	    'EndDate' => $endDate,
	    'PublisherId' => $publisherId,
	    'StartDate' => $startDate
	));
	return $response;	
}

function _affilinet_GetSalesLeadsStatistics($progamIds, $subId){
	// Send a request to the Logon Service to get an authentication token
	$soapLogon = new SoapClient(WSDL_LOGON);
	$token = $soapLogon->Logon(array(
	    'Username' => USERNAME,
	    'Password' => PASSWORD,
	    'WebServiceType' => 'Publisher'
	));
	 
	// Set parameters
	$startDate = strtotime("-1 year");
	$endDate = strtotime("today");
	$params = array(
	    'StartDate' => $startDate,
	    'EndDate' => $endDate,
	    'TransactionStatus' => 'All',
	    'ProgramIds' => $progamIds,
	    'SubId' => $subId,
	    'ProgramTypes' => 'All',
	    'MaximumRecords' => '1000',
	    'ValuationType' => 'DateOfRegistration'
	);
	 
	// Send a request to the Publisher Statistics Service
	$soapRequest = new SoapClient(WSDL_PUBSTATS);
	$response = $soapRequest->GetSalesLeadsStatistics(array(
	    'CredentialToken' => $token,
	    'GetSalesLeadsStatisticsRequestMessage' => $params
	));
	
	return $response;
}

function _affilinet_GetTransactions($programId, $subId) {
	// Send a request to the Logon Service to get an authentication token
	$soapLogon = new SoapClient(WSDL_LOGON);
	$token = $soapLogon->Logon(array(
	    'Username'  => USERNAME,
	    'Password'  => PASSWORD,
	    'WebServiceType' => 'Publisher'
	));
	 
	// Set page setting parameters
	$pageSettings = array(
	    'CurrentPage' => 1,
	    'PageSize' => 50,
	);
	 
	// Set transaction query parameters
	$startDate = strtotime("-1 year");
	$endDate = strtotime("today");
	$rateFilter = array(
	    'RateMode' => 'PayPerSale',
	    'RateNumber' => 1
	);
	$transactionQuery = array(
	    'StartDate' => $startDate,
	    'EndDate' => $endDate,
	    //'RateFilter' => $rateFilter,
	    'TransactionStatus' => 'All',
	    'ValuationType' => 'DateOfRegistration',
	    'SubId' => $subId,
	    'ProgramId' => $programId
	);
	 
	// Send a request to the Publisher Statistics Service
	$soapRequest = new SoapClient(WSDL_PUBSTATS);
	$response = $soapRequest->GetTransactions(array(
	    'CredentialToken' => $token,
	    'PageSettings' => $pageSettings,
	    'TransactionQuery' => $transactionQuery
	));
	
	return $response;
}

function _affilinet_GetBasketItems(){
	// Send a request to the Logon Service to get an authentication token
	$soapLogon = new SoapClient(WSDL_LOGON);
	$token = $soapLogon->Logon(array(
	    'Username' => USERNAME,
	    'Password' => PASSWORD,
	    'WebServiceType' => 'Publisher'
	));
	 
	// Set parameters
	$basketId = ''; // ID of the basket you want to retrieve the items from
	 
	// Send request to Publisher Statistics Service
	$soapRequest = new SoapClient(WSDL_PUBSTATS);
	$response = $soapRequest->GetBasketItems(array(
	    'CredentialToken' => $token,
	    'BasketId' => $basketId
	));
	
	return $response;
}
